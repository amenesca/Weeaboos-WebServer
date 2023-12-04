/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:56 by femarque          #+#    #+#             */
/*   Updated: 2023/12/04 12:20:45 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer() {
	this->_buffer[4096] = 0;
	this->_opt = 1;
	this->_socket_address_len = sizeof(_socket_address);
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

    return (0);
}

int WebServer::createSocket()
{
    if((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw socketError();
    return (0);
}

int WebServer::attachSocket()
{
    int	setsockopt_return;

    if ((setsockopt_return = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt))) < 0)
        throw setsockoptError();
    return (0);
}

int WebServer::bindSocket ()
{
	int bind_return;
	
    bzero(&_socket_address, sizeof(_socket_address));
    _socket_address.sin_family = AF_INET;
    _socket_address.sin_addr.s_addr = INADDR_ANY;
    _socket_address.sin_port = htons(PORT);

    if ((bind_return = bind(_socket_fd, (struct sockaddr*)&_socket_address, _socket_address_len)) < 0)
		throw bindError();
    return (0);
}

int WebServer::translateAddr(const char *addr)
{
	if(inet_pton(AF_INET, addr, &_socket_address.sin_addr) <= 0)
		throw std::runtime_error("Error on translation");
}

int WebServer::serverListen()
{
    int listen_return;
	
	if ((listen_return = listen(_socket_fd, 3)) < 0)
        throw listenError();
    return (0);
}

int WebServer::serverAccept()
{
    int accept_return;
	
	if ((accept_return = accept(_socket_fd, (struct sockaddr*)&_socket_address, &_socket_address_len)) < 0)
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
    close(_new_socket_fd);
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
