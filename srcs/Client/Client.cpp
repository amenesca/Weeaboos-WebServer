#include "./Client.hpp"

Client::Client() {
    _requestBuffer = new u_int8_t[MAX_BUFFER_SIZE + 1];
}

Client::~Client() {
    delete[] _requestBuffer;
}

u_int8_t **Client::getBufferAddress(void) {
    return (&_requestBuffer);
}

int Client::getClientSocket(void) const {
    return _clientSocket;
}

socklen_t Client::getClientAddrLen(void) const {
    return _client_addr_len;
}

SA Client::getClientAddr(void) const {
    return _client_addr;
}

ssize_t Client::getBytesRead(void) const {
    return _bytesRead;
}
