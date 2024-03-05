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
#include "../Response/Response.hpp"
#include "../RequestParser/RequestParser.hpp"

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
		this->_request = copy._request;
	}
	return *this;
}

std::string Client::getRequest(void) {
    return (_request);
}

sockaddr_in *Client::getClientAddrPointer(void) {
    return &_client_addr;
}

socklen_t *Client::getClientAddrLenPointer(void) {
    return &_client_addr_len;
}

int Client::getClientSocket(void) const {
    return _clientSocket;
}

ssize_t Client::getBytesRead(void) const {
    return _bytesRead;
}

void Client::setRequest(std::string request) {
    //tive que usa o new o negocio estava muito doido, dando segfault sem motivo
    _request = request;
}

void Client::setClientSocket(int clientSocket) {
    _clientSocket = clientSocket;
}

void Client::setClientAddrLen(socklen_t client_addr_len) {
    _client_addr_len = client_addr_len;
}

void Client::setBytesRead(ssize_t bytesRead) {
    _bytesRead = bytesRead;
}

std::string Client::uint8_to_string(const uint8_t* data, size_t size) {
    return std::string(reinterpret_cast<const char*>(data), size);
}

void Client::processRequest(const std::string& httpRequest, int *returnContentLenght, bool *requestComplete) {
    // Verifica se é uma solicitação POST
    size_t postPos = httpRequest.find("POST");
	size_t getPost = httpRequest.find("GET");
	if (getPost != std::string::npos) {
		*requestComplete = true;
		return;
	}
    if (postPos != std::string::npos) {

        // Recupera o valor Content-Length
        size_t contentLengthPos = httpRequest.find("Content-Length:");
        if (contentLengthPos != std::string::npos) {
            size_t start = contentLengthPos + std::string("Content-Length:").length();
            size_t end = httpRequest.find("\r\n", start);
            std::string contentLengthStr = httpRequest.substr(start, end - start);
            int contentLength = atoi(contentLengthStr.c_str());
            *returnContentLenght = contentLength;
        } else {
            std::cout << "Cabeçalho Content-Length não encontrado" << std::endl;
        }
    } else {}
}

int Client::receiveRequest(bool firstRequestLoop, pollfd *clientPoll)
{
	static bool	requestIsComplete = false;
	static int	AllBytesReceived = 0;
	static int	contentLenght = 0;

	_bytesRead = recv(clientPoll->fd, _buffer, sizeof(_buffer) - 1, 0);
    _stringBuffer = uint8_to_string(_buffer, _bytesRead);
	std::cout << "Após o recve" << std::endl;
	if (_bytesRead < 0) {
		std::cerr << "Erro ao receber dados do cliente, socket: " << clientPoll->fd << std::endl;
		return -1;
	} 
	else if (_bytesRead == 0) {
		std::cerr << "Erro: cliente desconectado, socket: " << clientPoll->fd << std::endl;
		return -1;
	}
	else if (requestIsComplete == false) {
		std::cout << "---Bytes Recebidos: \n" << _bytesRead << std::endl;
		std::cout << "---Buffer recebido: \n" << _stringBuffer << std::endl;
		_request.append(_stringBuffer.c_str(), _bytesRead);

		if (firstRequestLoop == true)
			processRequest(_stringBuffer, &contentLenght, &requestIsComplete);
		
		if (firstRequestLoop == false)
			AllBytesReceived = AllBytesReceived + _bytesRead;
		
		if (AllBytesReceived == contentLenght) {
			requestIsComplete = true;
		}

		if (requestIsComplete == true) {
			_request = _request + "\0";
			std::cout << "---Requisição Completa---\n" << _request << std::endl;
			
			requestIsComplete = false;
			AllBytesReceived = 0;
			contentLenght = 0;
			
			clientPoll->events = POLLOUT;
			return 0;
		}
	}
    return (0);
}

int Client::sendResponse(pollfd *clientPoll, std::vector<VirtualServer>& Configs)
{
	RequestParser	requestParser;
	size_t			virtualServerPosition;
	
	virtualServerPosition = -1;
	std::cout << "Antes do request parser" << std::endl;
	requestParser.parse(_request);
	std::cout << "Após do request parser" << std::endl;

	std::cout << "Antes de procurar os vservers" << std::endl;
	for (size_t v = 0; v < Configs.size(); v++) {
		std::cout << "dentro do for do vServer.size()" << std::endl;
		if (requestParser.getHeaders()["Host"] == Configs[v].getServerName()) {
			virtualServerPosition = v;
		}
	}
	std::cout << "Após procurar os vservers" << std::endl;

	Response makeResponse(requestParser, Configs[virtualServerPosition], *this);
	makeResponse.httpMethods();

    std::string response = makeResponse.getHttpMessage();

    send(clientPoll->fd, response.c_str(), response.size(), 0);
    


    return (0);
}