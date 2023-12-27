/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/22 14:57:07 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() {
	memset(_buffer, 0, MAX_BUFFER_SIZE);
	memset(_recbuffer, 0, MAX_BUFFER_SIZE);
	this->_opt = 1;
	_server_addr_len = sizeof(_server_addr);
	_client_addr_len = sizeof(_client_addr);
	memset(&_server_addr, 0, _server_addr_len);
	memset(&_client_addr, 0, _client_addr_len);
}

Socket::~Socket() {
    closeServer();
}

char *Socket::bin2hex(const unsigned char *input, size_t len)
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
}

int Socket::startServer() {
  try {
    createSocket();
    setServerOptions();
    bindSocket();
	//translateAddr(argv[1]);
    serverListen();
    acceptConnection();
	closeServer();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		closeServer();
	}
	return (0);
}

int Socket::createSocket() {
    if((_serversocket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw socketError();
	}
    return (0);
}

int Socket::setServerOptions() {
    if (setsockopt(_serversocket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
        throw setsockoptError();
	}
    return (0);
}

int Socket::bindSocket () {
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_addr.sin_port = htons(PORT);
	
	if (bind(_serversocket_fd, (SA*)&_server_addr, _server_addr_len) < 0) {
		throw bindError();
	}
    return (0);
}

int Socket::serverListen() {
	if (listen(_serversocket_fd, 10) < 0) {
        throw listenError();
	}
    return (0);
}

/*int Socket::acceptConnection() {
	struct pollfd fds[1];
	int pollReturn;
    fds[0].fd = _serversocket_fd;
    fds[0].events = POLLIN;
	
    while (1) {
        std::cout << "Waiting for connection on port " << PORT << std::endl;
        fflush(stdout);
		if ((pollReturn = poll(fds, 1, -1)) == -1) {
			std::cerr << "Error in poll" << std::endl;
            continue;
		}
		std::cout << "Poll: " << pollReturn << std::endl;
		if (fds[0].revents & POLLIN) {
			if ((_clientsocket_fd = accept(_serversocket_fd, (SA*)&_client_addr, &_client_addr_len)) < 0) {
				throw acceptError();
			}
		}*/

int Socket::acceptConnection()
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

			_valread = read(_mypollfds[i].fd, _recbuffer, MAX_BUFFER_SIZE - 1);

			if (_valread < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					std::cout << "Erro não bloqueante" << std::endl;
				// é pra ficar vazio mesmo, pra nao fazer nada, caso o contrário nao funciona
				} 
				else {
					std::cerr << "Read error: " << strerror(errno) << std::endl;
					std::cout << "Cliente desconectado, socket: " << _mypollfds[i].fd << std::endl;
					close(_mypollfds[i].fd);
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
					--i; // Revisita a posição movida para verificar eventos novamente
				}
			} else if (_valread == 0) {
				std::cout << "Cliente desconectado, socket: " << _mypollfds[i].fd << std::endl;
				close(_mypollfds[i].fd);
        		std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
				--nextClientId;
				--i; // Revisita a posição movida para verificar eventos novamente

    		} else {
				_recbuffer[_valread] = '\0';
				std::cout << "\n" << "Dados recebidos do cliente: " << _mypollfds[i].fd << "\n"\
				 << _recbuffer << std::endl;
			}
		
			memset(_recbuffer, 0, MAX_BUFFER_SIZE);
		
			// Configurar modo não bloqueio
			int flags_write = fcntl(_mypollfds[i].fd, F_GETFL, 0);
			fcntl(_mypollfds[i].fd, F_SETFL, flags_write | O_NONBLOCK);

//			Configurar para POLLOUT
			_mypollfds[i].events = POLLOUT;	
			if (_mypollfds[i].revents & POLLOUT)
			{
				snprintf((char*)_buffer, sizeof(_buffer), "HTTP/1.0 200 OK\r\n\r\nHello, World!\r\n");
				int bytes_writen = write(_mypollfds[i].fd, (char*)_buffer, strlen((char*)_buffer));
				if(bytes_writen <= 0)
				{
					std::cerr << "Erro de escrita: " << strerror(errno) << std::endl;
				
					close(_mypollfds[i].fd);
					std::swap(_mypollfds[i], _mypollfds[nextClientId - 1]);
					--nextClientId;
				}
				else {
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

//exercício de sockets
/*int Socket::translateAddr(const char *addr)
{
	if (inet_pton(AF_INET, addr, &_socket_address.sin_addr) <= 0)
    	throw std::runtime_error("Error on translation");
	if (connect(_socket_fd, (struct sockaddr*)&_socket_address, _socket_address_len) < 0)
    	throw std::runtime_error("Error on connection");
	std::sprintf(_buffer, "GET / HTTP/1.1\r\n\r\n");
	_sendbyte = strlen(_buffer);
	if (write(_socket_fd, _buffer, _sendbyte) != _sendbyte)
		throw std::runtime_error("Write error");
	memset(_recbuffer, 0, MAX_BUFFER_SIZE);
	while ((_valread = read(_socket_fd, _recbuffer, MAX_BUFFER_SIZE - 1)) > 0)
	{
		std::cout << _recbuffer;
	}
	if (_valread < 0)
		throw std::runtime_error("Read error");
	return (0);
}*/

/*int Socket::serverAccept()
{
	if (accept(_socket_fd, (struct sockaddr*)&_socket_address, &_socket_address_len) < 0)
        throw acceptError();
    return (0);
}

int Socket::serverRead()
{
    if ((_valread = read(_new_socket_fd, (void*)_buffer, MAX_BUFFER_SIZE)) < 0)
        throw readError();
    return (0);
}*/

void Socket::closeServer()
{
    close(_serversocket_fd);
    free(_hexbin);
    //close(_new_socket_fd);
    exit(0);
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
