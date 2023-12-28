/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/28 16:18:34 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

WebServer::WebServer()
    : _serverSocket(),
    _newClientSocket(),
    _opt(),
    _buffer(),
    _bytesRead(),
    _bytesSent(),
    _server_addr_len(),
    _client_addr_len(),
    _pollFds(),
    _server_addr(),
    _client_addr() {
    this->_opt = 1;
    _server_addr_len = sizeof(_server_addr);
    _client_addr_len = sizeof(_client_addr);
}

WebServer::~WebServer() {
    close(_serverSocket);
}

void WebServer::startServer() {
    createSocket();
    setServerOptions();
	configAddress();
    bindSocket();
    serverListen();
    acceptConnection();
}

int WebServer::createSocket() {
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverSocket== -1) {
        throw socketError();
	}
    return (0);
}

int WebServer::setServerOptions() {
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
        throw setsockoptError();
	}
    return (0);
}

void WebServer::configAddress() {
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(PORT);
	_server_addr.sin_addr.s_addr = INADDR_ANY;
}

int WebServer::bindSocket () {
	if (bind(_serverSocket, (SA*)&_server_addr, _server_addr_len) == -1) {
		throw bindError();
	}
    return (0);
}

int WebServer::serverListen() {
	if (listen(_serverSocket, 10) == -1) {
        throw listenError();
	}
    return (0);
}

int WebServer::acceptConnection()
{
	_pollFds.resize(MAX_CLIENTS + 1);
	_pollFds[0].fd = _serverSocket;
	_pollFds[0].events = POLLIN;
	
    while (true)
	{
        std::cout << "Waiting for connection on port " << PORT << std::endl;
	
		int pollReturn = poll(&_pollFds[0], _pollFds.size(), -1);
		if ( pollReturn == -1) {
			std::cerr << "Error in poll" << std::endl;
            break;
		}
		if (_pollFds[0].revents & POLLIN) {
			_newClientSocket = accept(_serverSocket, (SA*)&_client_addr, &_client_addr_len);
			if (_newClientSocket < 0) {
				std::cerr << "Error in accept" << std::endl;
			} else {
				std::cout << "Nova conexão aceita, socket: " << _newClientSocket << std::endl;
				int flags = fcntl(_newClientSocket, F_GETFL, 0);
				fcntl(_newClientSocket, F_SETFL, flags | O_NONBLOCK);
				_pollFds.push_back(pollfd());
       	 		_pollFds.back().fd = _newClientSocket;
        		_pollFds.back().events = POLLIN;
			}
		}
		for (size_t i = 1; i < _pollFds.size(); ++i) {
			if (_pollFds[i].revents & POLLIN)
			{
				_bytesRead = recv(_pollFds[i].fd, _buffer, sizeof(_buffer) - 1, 0);
				if (_bytesRead <= 0) {
                    if (_bytesRead < 0) {
                        std::cerr << "Erro ao receber dados do cliente, socket: " << _pollFds[i].fd << std::endl;
                    } else {
                        std::cerr << "Cliente desconectado, socket: " << _pollFds[i].fd << std::endl;
                    }
                    close(_pollFds[i].fd);
                    _pollFds.erase(_pollFds.begin() + i);
                    --i;
                } else {
                    _buffer[_bytesRead] = '\0';
                    std::cout << "Dados recebidos do cliente, socket: " << _pollFds[i].fd << "\n" << _buffer << std::endl;
                    _pollFds[i].events = POLLOUT;
                }
            }
			if (_pollFds[i].revents & POLLOUT)
			{
    			std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World\n";
    			_bytesSent = send(_pollFds[i].fd, response.c_str(), response.length(), 0);
				if (_bytesSent == -1) {
    				std::cerr << "Erro ao enviar a resposta para o cliente, socket: " << _pollFds[i].fd << std::endl;
				}
				close(_pollFds[i].fd);
				_pollFds.erase(_pollFds.begin() + i);
                --i;
    		}
		}
	}
	close(_serverSocket);
    return(0);
}

const char *WebServer::socketError::what() const throw() {
    std::string errorReturn =  "Error: socket(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *WebServer::setsockoptError::what() const throw() {
    std::string errorReturn =  "Error: setsockopt(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *WebServer::bindError::what() const throw() {
    std::string errorReturn =  "Error: bind(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *WebServer::listenError::what() const throw() {
    std::string errorReturn =  "Error: listen(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *WebServer::acceptError::what() const throw() {
    std::string errorReturn =  "Error: accept(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}

const char *WebServer::readError::what() const throw() {
    std::string errorReturn =  "Error: read(): " + std::string(strerror(errno));
    return (errorReturn.c_str());
}
