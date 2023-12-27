/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/27 16:30:33 by femarque         ###   ########.fr       */
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
    close(_serversocket_fd);
    //free(_hexbin);
}

/*char *WebServer::bin2hex(const unsigned char *input, size_t len)
{
    size_t resultlen;
    const char *hexits = "0123456789ABCDEF";
	
	if (input == NULL || len == 0) {
		return (NULL);
	}
	resultlen = (len * 3) + 1;
	_hexbin = (char*)malloc(resultlen);
	if (_hexbin == NULL) {
		return (NULL);
	}
	memset(_hexbin, 0, resultlen);
	
	for (size_t i = 0; i < len; i++) {
		_hexbin[i * 3] = hexits[input[i] >> 4];
		_hexbin[(i * 3) + 1] = hexits[input[i] & 0x0F];
		_hexbin[(i * 3) + 2] = ' ';
	}
	return (_hexbin);
}*/

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
				std::cerr << "Errot in accept" << std::endl;
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
		
		// Não estou dando loop no read ou write, estou dando loop na estrutura para verificar
		// se tem coisa para ler ou escrever nos sockets
		for (int i = 1; i < nextClientId; ++i) {

			_bytesRead = recv(_mypollfds[i].fd, _recbuffer, MAX_BUFFER_SIZE - 1, 0);

			if (_bytesRead <= 0) {
					std::cerr << "Erro ao receber a solicitação HTTP\n";
					std::cout << "Cliente desconectado, socket: " << _mypollfds[i].fd << std::endl;
					close(_mypollfds[i].fd);
					_mypollfds[i].fd = -1;
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
					--i; // Revisita a posição movida para verificar eventos novamente
			} else {
				_recbuffer[_bytesRead] = '\0';
				std::cout << "\n" << "Dados recebidos do cliente: " << _mypollfds[i].fd << "\n"\
				 << _recbuffer << std::endl;
			}
		
			//memset(_recbuffer, 0, MAX_BUFFER_SIZE);

			/*int flags_write = fcntl(_mypollfds[i].fd, F_GETFL, 0);
			fcntl(_mypollfds[i].fd, F_SETFL, flags_write | O_NONBLOCK);*/

			//Configurar para POLLOUT
			_mypollfds[i].events = POLLOUT;	
			if (_mypollfds[i].revents & POLLOUT)
			{
				//snprintf((char*)_buffer, sizeof(_buffer), "HTTP/1.0 200 OK\r\n\r\nHello, World!\r\n");
				std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World\n";
				_bytesSent = send(_mypollfds[i].fd, response.c_str(), response.length(), 0);
				if(_bytesSent <= 0)
				{
					std::cerr << "Erro de escrita: " << strerror(errno) << std::endl;
				
					close(_mypollfds[i].fd);
					_mypollfds[i].fd = -1;
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
				}
				else {
					std::cout << "Os dados foram escritos" << std::endl;
					//close(_mypollfds[i].fd);
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
