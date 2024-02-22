/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:24:46 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:25:14 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Client.hpp"

Client::Client() {
    _client_addr_len = sizeof(_client_addr);
}

Client::~Client() {
}

Client::Client(const Client& copy) {
	*this = copy;
}

Client& Client::operator=(const Client& copy) {
	if (this != &copy) {
		this->_bytesRead = copy._bytesRead;
		this->_client_addr = copy._client_addr;
		this->_client_addr_len = copy._client_addr_len;
		this->_clientSocket = copy._clientSocket;
		this->_requestBuffer1 = copy._requestBuffer1;
	}
	return *this;
}

std::string Client::getBuffer(void) {
    return (_requestBuffer1);
}

SA *Client::getClientAddrPointer(void) {
    return &_client_addr;
}

socklen_t *Client::getClientAddrLenPointer(void) {
    return &_client_addr_len;
}

int Client::getClientSocket(void) const {
    return _clientSocket;
}

int Client::getBytesRead(void) const {
    return _bytesRead;
}

void Client::setRequestBuffer(std::string requestBuffer) {
    //tive que usa o new o negocio estava muito doido, dando segfault sem motivo
    _requestBuffer1 = requestBuffer;
}

void Client::setClientSocket(int clientSocket) {
    _clientSocket = clientSocket;
}

void Client::setClientAddrLen(socklen_t client_addr_len) {
    _client_addr_len = client_addr_len;
}

void Client::setBytesRead(int bytesRead) {
    _bytesRead = bytesRead;
}