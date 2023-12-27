/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/27 20:48:36 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

WebServer::WebServer() : _buffer(), _server_addr(), _client_addr() {
	this->_opt = 1;
	_server_addr_len = sizeof(_server_addr);
	_client_addr_len = sizeof(_client_addr);
}
WebServer::~WebServer() {
    close(_serversocket_fd);
}

int WebServer::startServer() {
    createSocket();
    setServerOptions();
    bindSocket();
    serverListen();
    acceptConnection();
	return (0);
}

int WebServer::createSocket() {
    if((_serversocket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw socketError();
	}
    return (0);
}

int WebServer::setServerOptions() {
    if (setsockopt(_serversocket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
        throw setsockoptError();
	}
    return (0);
}

int WebServer::bindSocket () {
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_addr.sin_port = htons(PORT);
	
	if (bind(_serversocket_fd, (SA*)&_server_addr, _server_addr_len) < 0) {
		throw bindError();
	}
    return (0);
}

int WebServer::serverListen() {
	if (listen(_serversocket_fd, 10) < 0) {
        throw listenError();
	}
    return (0);
}

int WebServer::acceptConnection()
{
	int pollReturn;
	int nextClientId = 1;

	_mypollfds.resize(MAX_CLIENTS + 1);
	_mypollfds[0].fd = _serversocket_fd; // A posição 0 vai ser sempre do servidor.
	_mypollfds[0].events = POLLIN;
	
    while (true)
	{
        std::cout << "Waiting for connection on port " << PORT << std::endl;
	
		// Monitora desde a primeira posição até a quantidade de clientes conectados
		pollReturn = poll(&_mypollfds[0], nextClientId, -1);
		if ( pollReturn == -1) {
			std::cerr << "Error in poll" << std::endl;
            break;
		}

		// Verificar se há uma nova conexão
		if (_mypollfds[0].revents & POLLIN) {

			_newclientsocket_fd = accept(_serversocket_fd, (SA*)&_client_addr, &_client_addr_len);
			
			if (_newclientsocket_fd < 0) {
				std::cerr << "Error in accept" << std::endl;
			} else {
				std::cout << "Nova conexão aceita, socket: " << _newclientsocket_fd << std::endl;

				// Adicionar novo cliente na estrutura
				if (nextClientId < MAX_CLIENTS + 1) {

					std::cout << "Novo Cliente Adicionado a estrutura" << std::endl; 

					_mypollfds[nextClientId].fd = _newclientsocket_fd;
					// Evento para monitorar é o POLLOUT nos clientes
					_mypollfds[nextClientId].events = POLLIN;
					_clientsockets_fd.push_back(_newclientsocket_fd);
					++nextClientId;

					// Configurar para modo não bloqueante
					int flags = fcntl(_newclientsocket_fd, F_GETFL, 0);
					fcntl(_newclientsocket_fd, F_SETFL, flags | O_NONBLOCK);
				} 
				else {
					std::cerr << "Limite máximo de clientes atingido." << std::endl;
					close(_newclientsocket_fd);
				}
			}
		}
		for (int i = 1; i < nextClientId; ++i) {
			_bytesRead = read(_mypollfds[i].fd, _buffer, sizeof(_buffer));
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
				std::cout << "Cliente desconectado, socket: " << _mypollfds[i].fd << std::endl;
				//close(_mypollfds[i].fd);
				std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
				--nextClientId;
				//--i; // Revisita a posição movida para verificar eventos novamente
			} else {
				_buffer[_bytesRead] = '\0';
				std::cout << "\n" << "Dados recebidos do cliente: " << _mypollfds[i].fd << "\n"\
				<< _buffer << std::endl;
			}
			memset(_buffer, 0, MAX_BUFFER_SIZE);

			_mypollfds[i].events = POLLOUT;	
			if (_mypollfds[i].revents & POLLOUT) {
    			std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World\n";
				_bytesSent = write(_mypollfds[i].fd, response.c_str(), response.length());
    			if (_bytesSent <= 0) {
        			close(_mypollfds[i].fd);
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
    			} else {
        			std::cout << "Os dados foram escritos" << std::endl;
					close(_mypollfds[i].fd);
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
					_mypollfds[i].events = POLLIN;
    			}
			}
		}
	}
	close(_serversocket_fd);
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
