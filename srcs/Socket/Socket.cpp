/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/01 13:11:41 by femarque         ###   ########.fr       */
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

std::string Socket::uint8_to_string(const uint8_t* data, size_t size) {
    return std::string(reinterpret_cast<const char*>(data), size);
}

void Socket::startServer() {
    createSocket();
    setServerOptions();
	configAddress();
    bindSocket();
    serverListen();
    Connection();
}

int Socket::createSocket() {
	_serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
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
    Client newClient;
	newClient._client_addr_len = sizeof(newClient._client_addr);
	newClient._clientSocket = -1;
    newClient._request = "";
    newClient._bytesRead = 0;

    newClient._clientSocket = accept(_serverSocket, (SA*)&newClient._client_addr, &newClient._client_addr_len);
    if (newClient._clientSocket < 0) {
        std::cerr << "Error in accept" << std::endl;
        newClient.~Client();
    } else {
        int flags = fcntl(newClient._clientSocket, F_GETFL, 0);
        fcntl(newClient._clientSocket, F_SETFL, flags | O_NONBLOCK);
        _pollFds.push_back(pollfd());
        _pollFds.back().fd = newClient._clientSocket;
        _pollFds.back().events = POLLIN;
        _client.push_back(newClient);
        std::cout << "Nova conexão aceita, socket: " << newClient._clientSocket << std::endl;
    }
    return (0);
}

int Socket::receiveRequest(size_t *i)
{
	size_t j = *i - 1;

	_client[j]._bytesRead = recv(_pollFds[*i].fd, _buffer, sizeof(_buffer) - 1, 0);
    std::string bufferConverted = uint8_to_string(_buffer, _bytesRead);

    if (_client[j]._bytesRead <= 0) {
        if (_client[j]._bytesRead < 0) {
            std::cerr << "Erro ao receber dados do cliente, socket: " << _pollFds[*i].fd << std::endl;
        } else {
            std::cerr << "Cliente desconectado, socket: " << _pollFds[*i].fd << std::endl;
        }
        close(_pollFds[*i].fd);
        _pollFds.erase(_pollFds.begin() + *i);
        _client.erase(_client.begin() + j);
        --*i;
    } else {
    	_buffer[_client[j]._bytesRead] = '\0';
        _requestBuffer.append(bufferConverted.c_str(), _bytesRead);
        std::cout << "Dados recebidos do cliente, socket: " << _pollFds[*i].fd << "\n" << _requestBuffer << std::endl;
        _client[j]._request = _requestBuffer;
        _requestBuffer.clear();
        memset(_buffer, '\0', MAX_BUFFER_SIZE + 1);
        _pollFds[*i].events = POLLOUT;
    }
    return (0);
}

int Socket::sendResponse(size_t *i)
{
	RequestParser	requestParser;
	size_t			virtualServerPosition;
	size_t			j = *i - 1;

	virtualServerPosition = -1;

	requestParser.parse(_client[j]._request);
	
	for (size_t v = 0; v < _vServers.size(); v++) {
		if (requestParser.getHeaders()["Host"] == _vServers[v]._serverName) {
			virtualServerPosition = v;
			break;
		}
	}

	Response makeResponse(requestParser, _client[j]);
	makeResponse.httpMethods(_vServers[virtualServerPosition]);

    std::string response = makeResponse.getHttpMessage();

    send(_pollFds[*i].fd, response.c_str(), response.size(), 0);
    
    close(_pollFds[*i].fd);
    _pollFds.erase(_pollFds.begin() + *i);
    _client.erase(_client.begin() + j);
    --*i;

    return (0);
}

int Socket::Connection()
{
	struct pollfd serverPoll;
	serverPoll.fd = _serverSocket;
	serverPoll.events = POLLIN;
	_pollFds.push_back(serverPoll);

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
			if (_pollFds[i].revents & POLLIN)
			{
				receiveRequest(&i);
            }
			if (_pollFds[i].revents & POLLOUT)
			{
                sendResponse(&i);
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