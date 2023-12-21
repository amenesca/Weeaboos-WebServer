/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/20 19:33:36 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket() {
	this->_buffer[MAX_BUFFER_SIZE - 1] = 0;
	this->_opt = 1;
	_server_addr_len = sizeof(_server_addr);
	_client_addr_len = sizeof(_client_addr);
	memset(&_server_addr, 0, _server_addr_len);
	memset(&_client_addr, 0, _client_addr_len);
}

Socket::~Socket() {
    closeServer();
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

void Socket::readRequest() {
	memset(_recbuffer, 0, MAX_BUFFER_SIZE);
	while ((_valread = read(_clientsocket_fd, _recbuffer, MAX_BUFFER_SIZE - 1)) > 0) {
		std::cout << _recbuffer << std::endl;
		if (_recbuffer[_valread - 1] == '\n') {
			break ;
		}
		memset(_recbuffer, 0, MAX_BUFFER_SIZE);
	}
	if (_valread < 0) {
		throw std::runtime_error("Read error");
	}
}

void Socket::sendResponse() {
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
}

int Socket::acceptConnection() {
	struct pollfd mypoll;
	memset(&mypoll, 0, sizeof(mypoll));
	int pollReturn;
    mypoll.fd = _serversocket_fd;
    mypoll.events = POLLIN;
	
    while (1) {
        std::cout << "Waiting for connection on port " << PORT << std::endl;
        fflush(stdout);
		if ((pollReturn = poll(&mypoll, 1, -1)) == -1) {
			std::cerr << "Error in poll" << std::endl;
            continue;
		}
        if (mypoll.revents & POLLIN) {
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

			readRequest();
			sendResponse();
		
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
