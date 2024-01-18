#include "./Client.hpp"

Client::Client() {
    _client_addr_len = sizeof(_client_addr);
}

Client::~Client() {
}

std::string *Client::getBufferAddress(void) {
    return (&_requestBuffer);
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

ssize_t Client::getBytesRead(void) const {
    return _bytesRead;
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