/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/20 16:28:58 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

WebServer::WebServer() {
	this->_buffer[MAX_BUFFER_SIZE - 1] = 0;
	this->_opt = 1;
	_server_addr_len = sizeof(_server_addr);
	_client_addr_len = sizeof(_client_addr);
	memset(&_server_addr, 0, _server_addr_len);
	memset(&_client_addr, 0, _client_addr_len);
}

WebServer::~WebServer() {
    closeServer();
}

char *WebServer::bin2hex(const unsigned char *input, size_t len)
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

int WebServer::startServer() {
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

/*int WebServer::acceptConnection() {
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

int WebServer::acceptConnection() {
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
        if (fds[0].revents & POLLIN) {
			if ((_clientsocket_fd = accept(_serversocket_fd, (SA*)&_client_addr, &_client_addr_len)) < 0) {
				throw acceptError();
			}
		}
		_pid = fork();
		if (_pid < 0) {
			std::cerr << "Error on forking process" << std::endl;
			close(_clientsocket_fd);
			continue ;
		} else if (_pid == 0) {
			char client_address[MAX_BUFFER_SIZE + 1];
			inet_ntop(AF_INET, &_client_addr, client_address, MAX_BUFFER_SIZE);
			printf("Client connection: %s\n", client_address);
			
			memset(_recbuffer, 0, MAX_BUFFER_SIZE);
			while ((_valread = read(_clientsocket_fd, _recbuffer, MAX_BUFFER_SIZE - 1)) > 0) {
				//fprintf(stdout, "\n%s\n\n%s", bin2hex(_recbuffer, _valread), _recbuffer);
				std::cout << "\n" << bin2hex(_recbuffer, _valread) << "\n\n" << _recbuffer << std::endl;
				if (_recbuffer[_valread - 1] == '\n') {
					break ;
				}
				memset(_recbuffer, 0, MAX_BUFFER_SIZE);
			}
			if (_valread < 0) {
				throw std::runtime_error("Read error");
			}
			snprintf((char*)_buffer, sizeof(_buffer), "HTTP/1.0 200 OK\r\n\r\n<!DOCTYPE html>\
			<html>\
			\
			<head>\
			<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\
			<title>Snowman Productions</title>\
			</head>\
			<body>\
			<h1>&#9731;Snowman Productions<sup>&reg;</sup></h1>\
			</body>\
			\
			</html>");
			write(_clientsocket_fd, (char*)_buffer, strlen((char*)_buffer));
			close(_clientsocket_fd);
			_client_addr_len = sizeof(_client_addr);
			memset(&_client_addr, 0, _client_addr_len);
			//close(_serversocket_fd);
		} else {
			close(_clientsocket_fd);
			if (waitpid(_pid, &_waitpid_status, 0) == -1) {
                std::cerr << "Error waiting for child process" << std::endl;
			}
		}
	}
	close(_serversocket_fd);
    return(0);
}

//exercício de sockets
/*int WebServer::translateAddr(const char *addr)
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

/*int WebServer::serverAccept()
{
	if (accept(_socket_fd, (struct sockaddr*)&_socket_address, &_socket_address_len) < 0)
        throw acceptError();
    return (0);
}

int WebServer::serverRead()
{
    if ((_valread = read(_new_socket_fd, (void*)_buffer, MAX_BUFFER_SIZE)) < 0)
        throw readError();
    return (0);
}*/

void WebServer::closeServer()
{
    close(_serversocket_fd);
    free(_hexbin);
    //close(_new_socket_fd);
    exit(0);
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
