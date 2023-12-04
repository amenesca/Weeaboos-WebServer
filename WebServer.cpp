/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/04 13:46:27 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer() {
	this->_buffer[MAX_BUFFER_SIZE] = 0;
	this->_opt = 1;
	_socket_address_len = sizeof(_socket_address);
	bzero(&_socket_address, _socket_address_len);
}

WebServer::~WebServer() {
    closeServer();
}

int WebServer::startServer(int argc, char **argv)
{
	(void)argc;
    createSocket();
    attachSocket();
    bindSocket();
	translateAddr(argv[1]);
    serverListen();
    serverAccept();
	closeServer();

    return (0);
}

int WebServer::createSocket()
{
    if(_socket_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
        throw socketError();
    return (0);
}

int WebServer::attachSocket()
{
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0)
        throw setsockoptError();
    return (0);
}

int WebServer::bindSocket ()
{
    _socket_address.sin_family = AF_INET;
    _socket_address.sin_addr.s_addr = INADDR_ANY;
    _socket_address.sin_port = htons(PORT);

    if (bind(_socket_fd, (struct sockaddr*)&_socket_address, _socket_address_len) < 0)
		throw bindError();
    return (0);
}

int WebServer::translateAddr(const char *addr)
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
	while ((_n = read(_socket_fd, _recbuffer, MAX_BUFFER_SIZE - 1)) > 0)
	{
		std::cout << _recbuffer;
	}
	if (_n < 0)
		throw std::runtime_error("Read error");
	return (0);
}

int WebServer::serverListen()
{
	if ((listen(_socket_fd, 3)) < 0)
        throw listenError();
    return (0);
}

int WebServer::serverAccept()
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
}

void WebServer::closeServer()
{
    close(_socket_fd);
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
