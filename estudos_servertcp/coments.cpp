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

// Parte que imprime o endereço do cliente
			// char client_address[MAX_BUFFER_SIZE + 1];
			// inet_ntop(AF_INET, &_client_addr, client_address, MAX_BUFFER_SIZE);
			// printf("Client connection: %s\n", client_address);

// bin2hex 
//        char *bin2hex(const unsigned char *input, size_t len);


// char *Socket::bin2hex(const unsigned char *input, size_t len)
// {
//     size_t resultlen;
//     const char *hexits = "0123456789ABCDEF";
	
// 	if (input == NULL || len == 0) {
// 		return (NULL);
// 	}
// 	resultlen = (len * 3) + 1;
// 	_hexbin = (char*)malloc(resultlen);
// 	if (_hexbin == NULL) {
// 		return (NULL);
// 	}
// 	memset(_hexbin, 0, resultlen);
	
// 	for (size_t i = 0; i < len; i++) {
// 		_hexbin[i * 3] = hexits[input[i] >> 4];
// 		_hexbin[(i * 3) + 1] = hexits[input[i] & 0x0F];
// 		_hexbin[(i * 3) + 2] = ' ';
// 	}
// 	return (_hexbin);
// }
