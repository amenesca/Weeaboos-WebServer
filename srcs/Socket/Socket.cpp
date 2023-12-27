/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/27 19:19:52 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

WebServer::WebServer() {
	memset(_buffer, 0, MAX_BUFFER_SIZE);
	memset(_recbuffer, 0, MAX_BUFFER_SIZE);
	memset(&_server_addr, 0, _server_addr_len);
	memset(&_client_addr, 0, _client_addr_len);
	this->_opt = 1;
	_server_addr_len = sizeof(_server_addr);
	_client_addr_len = sizeof(_client_addr);
}

WebServer::~WebServer() {
    close(_serverSocket);
}

int WebServer::startServer() {
    createSocket();
    setServerOptions();
	configAddress();
    bindSocket();
    serverListen();
    acceptConnection();
	return (0);
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

void WebServer::handleRequest(int i, int clientSocket) {
	 if (clientSocket < 0) {
        std::cerr << "Erro no socket do cliente\n";
        close(clientSocket);
        return ;
    }
	_bytesRead = recv(clientSocket, _buffer, sizeof(_buffer), 0);
	if (_bytesRead < 0) {
		if (errno == ECONNRESET) {
        std::cerr << "Conexão fechada pelo cliente\n";
    	} else if (errno == EOPNOTSUPP) {
        	std::cerr << "Operação não suportada pelo tipo de socket\n";
    	} else if (errno == EBADF) {
        	std::cerr << "Descritor de arquivo do socket não é válido\n";
    	} else {
        	std::cerr << "Erro ao receber a solicitação HTTP. Erro: " << errno << "\n";
    	}
		/*std::swap(_pollFds[i], _pollFds[nextClientId - 1]);
		--nextClientId;
		--i;*/
		close(clientSocket);
		return ;
	} else if (_bytesRead == 0) {
        std::cerr << "Conexão fechada pelo cliente\n";
        close(clientSocket);
        return;
    }
    std::cout << "Dados recebidos do cliente: " << _bytesRead << " bytes\n";
	_buffer[_bytesRead] = '\0';
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World\n";
	_pollFds[i].events = POLLOUT;
	int pollResult = poll(&_pollFds[i], 1, 0);
	if (pollResult > 0 && (_pollFds[i].revents & POLLOUT)) {
		send(clientSocket, response.c_str(), response.length(), 0);
	}
	close(clientSocket);
}

int WebServer::acceptConnection()
{
	int pollReturn;
	int nextClientId = 1;

	_pollFds.resize(MAX_CLIENTS + 1);
	_pollFds[0].fd = _serverSocket;
	_pollFds[0].events = POLLIN;
	
    while (true)
	{
		pollReturn = poll(_pollFds.data(), nextClientId, -1);
		if (pollReturn == -1) {
			std::cerr << "Error in poll" << std::endl;
            break;
		}
		for (size_t i = 0; i < _pollFds.size(); ++i) {
			if (_pollFds[0].revents & POLLIN) {
				if (_pollFds[i].fd == _serverSocket) {
					_newClientSocket = accept(_serverSocket, (SA*)&_client_addr, &_client_addr_len);	
					if (_newClientSocket == -1) {
						std::cerr << "Error in accept\n";
						continue ;
					} else {
						std::cout << "Nova conexão aceita, socket: " << _newClientSocket << std::endl;
						if (nextClientId < MAX_CLIENTS + 1) {
							std::cout << "Novo Cliente Adicionado a estrutura" << std::endl; 
							_pollFds[nextClientId].fd = _newClientSocket;
							_pollFds[nextClientId].events = POLLIN;
							_clientSockets.push_back(_newClientSocket);
							++nextClientId;
						} 
						else {
							std::cerr << "Limite máximo de clientes atingido." << std::endl;
							close(_newClientSocket);
						}
					}
				} else {
					handleRequest(i, _pollFds[i].fd);
					_pollFds.erase(_pollFds.begin() + i);
				}
			}
		}
	}
	close(_serverSocket);
	for (size_t i = 0; i < _pollFds.size(); ++i) {
        close(_pollFds[i].fd);
    }
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
