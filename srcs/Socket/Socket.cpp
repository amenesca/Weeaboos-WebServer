/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/01 15:56:26 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() 
    : _serverSocket(),
    _opt(),
    _buffer(),
    _server_addr_len(),
    _pollFds(),
    _server_addr()
{
    this->_opt = 1;
    _server_addr_len = sizeof(_server_addr);
	_clients = new Client[92];
	_clientsCount = 0;
}

Socket::~Socket() {
    close(_serverSocket);
}

void Socket::setVServers(std::vector<VirtualServer>& vServers) {
	_vServers = vServers;
}

void Socket::startServer() {
    createSocket();
    setServerOptions();
	configAddress();
    bindSocket();
    serverListen();
    Connection();
}

int Socket::configVServers(std::string configFilePath) {
    this->configs.setConfigFilePath(configFilePath);
	this->configs.initConfig();
	_vServers = this->configs.getVServers();
    return 0;
}

int Socket::createSocket() {
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket== -1) {
        throw socketError();
	}
    return (0);
}

int Socket::setServerOptions() {
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
        throw setsockoptError();
	}
    return (0);
}

void Socket::configAddress() {
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(_vServers[0]._port);
	_server_addr.sin_addr.s_addr = INADDR_ANY;
}

int Socket::bindSocket () {
	if (bind(_serverSocket, (SA*)&_server_addr, _server_addr_len) == -1) {
		throw bindError();
	}
    return (0);
}

int Socket::serverListen() {
	if (listen(_serverSocket, 10) == -1) {
        throw listenError();
	}
    return (0);
}

int Socket::acceptConnection() 
{
    Client	newClient;
	int 	newClientSocket;

	newClientSocket = accept(_serverSocket, (SA*)newClient.getClientAddrPointer(), newClient.getClientAddrLenPointer());
    newClient.setClientSocket(newClientSocket);

    if (newClient.getClientSocket() < 0) {
        std::cerr << "Error in accept" << std::endl;
        newClient.~Client();
    } else {
        int flags = fcntl(newClient.getClientSocket(), F_GETFL, 0);
        fcntl(newClient.getClientSocket(), F_SETFL, flags | O_NONBLOCK);
        _pollFds.push_back(pollfd());
        _pollFds.back().fd = newClient.getClientSocket();
        _pollFds.back().events = POLLIN;
		for (size_t i = 0; i < MAX_CLIENTS; i++) {
			if (_clients->getOccupied() == false) {
        		_clients[i] = newClient;
				_clientsCount++;
				_clients[i].setOccupied();
				break;
			}
		}
        std::cout << "Nova conexão aceita, socket: " << newClient.getClientSocket() << std::endl;
		std::cout << "Printando VServers\n" << std::endl;
		for (size_t i = 0; i < _vServers.size(); i++) {
			std::cout << _vServers[i]._serverName << std::endl; 
		}
    }
    return (0);
}

int Socket::Connection()
{
	_pollFds.resize(MAX_CLIENTS + 1);
	_pollFds[0].fd = _serverSocket;
	_pollFds[0].events = POLLIN;
    cgiHandler cgi;
	
    while (true)
	{
        std::cout << "Waiting for connection on port " << _vServers[0]._port << std::endl;
	
		int pollReturn = poll(&_pollFds[0], _pollFds.size(), -1);
		if ( pollReturn == -1) {
			std::cerr << "Error in poll" << std::endl;
            break;
		}
		if (_pollFds[0].revents & POLLIN) {
            acceptConnection();
		}
		for (size_t i = 1; i < _pollFds.size(); ++i) {
			// o pollfds usamos sempre a partir do i , o clients usamos a partir do j que é i - 1
			size_t j = i - 1;
			
			if (_pollFds[i].revents & POLLIN)
			{
				std::cout << "Receive Request" << std::endl;
				if (_clients[j].receiveRequest(i, &_pollFds) == -1) {
					close(_pollFds[i].fd);
 					_pollFds.erase(_pollFds.begin() + i);
 					--i;
					--_clientsCount;
				}
				std::cout << "Após o receive Request" << std::endl;
            }
			if (_pollFds[i].revents & POLLOUT)
			{
				std::cout << "Antes do send Response" << std::endl;
                _clients[j].sendResponse(i, &_pollFds, _vServers);
				std::cout << "Após o send Response" << std::endl;
				std::cout << 7 << std::endl;
				close(_pollFds[i].fd);
				std::cout << 8 << std::endl;

    			_pollFds.erase(_pollFds.begin() + i);
				std::cout << 9 << std::endl;
				std::cout << 10 << std::endl;
				--i;
				--_clientsCount;
    		}
		}
	}
	close(_serverSocket);
    return(0);
}

const char *Socket::socketError::what() const throw() {
    std::string errorReturn =  "Error: socket(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *Socket::setsockoptError::what() const throw() {
    std::string errorReturn =  "Error: setsockopt(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *Socket::bindError::what() const throw() {
    std::string errorReturn =  "Error: bind(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *Socket::listenError::what() const throw() {
    std::string errorReturn =  "Error: listen(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *Socket::acceptError::what() const throw() {
    std::string errorReturn =  "Error: accept(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *Socket::readError::what() const throw() {
    std::string errorReturn =  "Error: read(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}