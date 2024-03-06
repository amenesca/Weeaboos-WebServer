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

Client::Client()
:	_clientSocket(-1), // Inicializa _clientSocket com -1
	_client_addr_len(sizeof(_client_addr)), // Inicializa _client_addr_len com o tamanho de _client_addr
    _request(), // Inicializa _request como uma string vazia
    _stringBuffer(), // Inicializa _stringBuffer como uma string vazia
    _bytesRead(0), // Inicializa _bytesRead como zero
	_occupied(false)
{
	 memset(_buffer, 0, sizeof(_buffer));
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
		this->_occupied = copy._occupied;
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

void Client::setOccupied() {
	_occupied = true;
}

bool Client::getOccupied() {
	return _occupied;
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

int Client::receiveRequest(size_t i, std::vector<pollfd> *pollFds)
{
	std::cout << '1' << std::endl;
	_bytesRead = recv((*pollFds)[i].fd, _buffer, sizeof(_buffer) - 1, 0);
	std::cout << '2' << std::endl;
	std::cout << '3' << std::endl;
    if (_bytesRead <= 0) {
        if (_bytesRead < 0) {
            std::cerr << "Erro ao receber dados do cliente, socket: " << (*pollFds)[i].fd << std::endl;
        } else {
            std::cerr << "Cliente desconectado, socket: " << (*pollFds)[i].fd << std::endl;
        }
		return -1;
    } else {
		std::cout << '4' << std::endl;
       _buffer[_bytesRead] = '\0';
		std::cout << '5' << std::endl;

        _request.append(_buffer, _bytesRead);
		std::cout << '6' << std::endl;
        std::cout << "Dados recebidos do cliente, socket: " << (*pollFds)[i].fd << "\n" << _request << std::endl;
//        memset(this->_buffer, '\0', MAX_BUFFER_SIZE + 1); ***segfaultando
        (*pollFds)[i].events = POLLOUT;
    }
    return (0);
}

int Client::sendResponse(size_t i, std::vector<pollfd> *pollFds, std::vector<VirtualServer>& Configs)
{
	RequestParser	requestParser;
	size_t			virtualServerPosition;
	
	virtualServerPosition = -1;

	requestParser.parse(_request);
	
	for (size_t v = 0; v < Configs.size(); v++) {
		if (requestParser.getHeaders()["Host"] == Configs[v]._serverName) {

			virtualServerPosition = v;
		}
	}

	Response makeResponse(requestParser, Configs[virtualServerPosition], *this);
	makeResponse.httpMethods();

    std::string response = makeResponse.getHttpMessage();

    send((*pollFds)[i].fd, response.c_str(), response.size(), 0);

    return (0);
}