/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/07 11:32:57 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
    : _serverSocket(),
    _newClientSocket(),
    _opt(),
    _buffer(),
    _bytesRead(),
    _server_addr_len(),
    _client_addr_len(),
    _pollFds(),
    _server_addr(),
    _client_addr() {
    this->_opt = 1;
    _server_addr_len = sizeof(_server_addr);
    _client_addr_len = sizeof(_client_addr);
}

Socket::~Socket() {
    close(_serverSocket);
}

void Socket::setVServers(std::vector<VirtualServer> vServers) {
	_vServers = vServers;
}

void Socket::startServer(char **envp) {
    createSocket();
    setServerOptions();
	configAddress();
    bindSocket();
    serverListen();
    Connection(envp);
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
	_server_addr.sin_port = htons(_vServers[0].getPort());
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

int Socket::acceptConnection() {
    Client newClient;

    newClient.setClientSocket(accept(_serverSocket, (SA*)newClient.getClientAddrPointer(), newClient.getClientAddrLenPointer()));
    if (newClient.getClientSocket() < 0) {
        std::cerr << "Error in accept" << std::endl;
        newClient.~Client();
    } else {
        int flags = fcntl(newClient.getClientSocket(), F_GETFL, 0);
        fcntl(newClient.getClientSocket(), F_SETFL, flags | O_NONBLOCK);
        _pollFds.push_back(pollfd());
        _pollFds.back().fd = newClient.getClientSocket();
        _pollFds.back().events = POLLIN;
        newClient.setClientSocket(newClient.getClientSocket());
        _clients.push_back(newClient);
        std::cout << "Nova conexão aceita, socket: " << newClient.getClientSocket() << std::endl;
    }
    return (0);
}

int Socket::receiveRequest(size_t *i) {
    int j = *i - 1;

    _clients[j].setBytesRead(recv(_pollFds[*i].fd, _buffer, sizeof(_buffer) - 1, 0));
    if (_clients[j].getBytesRead() <= 0) {
        if (_clients[j].getBytesRead() < 0) {
            std::cerr << "Erro ao receber dados do cliente, socket: " << _pollFds[*i].fd << std::endl;
        } else {
            std::cerr << "Cliente desconectado, socket: " << _pollFds[*i].fd << std::endl;
        }
        close(_pollFds[*i].fd);
        _pollFds.erase(_pollFds.begin() + *i);
        _clients.erase(_clients.begin() + j);
        --*i;
    } else {
       _buffer[_clients[j].getBytesRead()] = '\0';
        std::cout << "Dados recebidos do cliente, socket: " << _pollFds[*i].fd << "\n" << std::endl;
        _requestBuffer += std::string(_buffer);
        _clients[j].setRequestBuffer(_requestBuffer);
        _requestBuffer.clear();
        memset(_buffer, '\0', MAX_BUFFER_SIZE + 1);
        _pollFds[*i].events = POLLOUT;
    }
    return (0);
}

int Socket::sendResponse(size_t *i, char **envp)
{
	cgiHandler		cgi;
	RequestParser	requestParser;
	size_t			virtualServerPosition;
	int				j;
	
	j = *i - 1;
	virtualServerPosition = -1;

	requestParser.parse(*_clients[j].getBuffer());
	
	for (size_t v = 0; v < _vServers.size(); v++) {
		if (requestParser.getHeaders()["Host"] == _vServers[v].getServerName()) {

			std::cout << _vServers[v].getServerName() << std::endl;
			std::cout << requestParser.getHeaders()["Host"] << std::endl;
			
			std::cout << "DEU BOM!!" << std::endl;
			virtualServerPosition = v;
		}
	}

	Response	makeResponse(requestParser, _vServers[virtualServerPosition]);
//	makeResponse.httpMethods(); // dando erro

    cgi.configCgi(_pollFds[*i].fd, envp);
    close(_pollFds[*i].fd);
    std::cout << "Dentro do send response\n\n" << *_clients[j].getBuffer() << std::endl;

	std::cout << "DELETE CLIENTS" << std::endl;

    delete _clients[j].getBuffer();

	std::cout << "AFTER DELETE CLIENTS" << std::endl;


    _pollFds.erase(_pollFds.begin() + *i);
    _clients.erase(_clients.begin() + (j));
    --*i;

	std::cout << "AFTER LOOP" << std::endl;

    return (0);
}

int Socket::Connection(char **envp)
{
	_pollFds.resize(MAX_CLIENTS + 1);
	_pollFds[0].fd = _serverSocket;
	_pollFds[0].events = POLLIN;
    cgiHandler cgi;
	
    while (true)
	{
        std::cout << "Waiting for connection on port " << _vServers[0].getPort() << std::endl;
	
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
			if (_pollFds[i].revents & POLLIN)
			{
				receiveRequest(&i);
            }
			if (_pollFds[i].revents & POLLOUT)
			{
                sendResponse(&i, envp);
    		}
		}
	}
	close(_serverSocket);
    return(0);
}

int const &Socket::getClientSocket() {
    return (_newClientSocket);
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