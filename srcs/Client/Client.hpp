/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:56 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:24:57 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Includes.hpp"
#include "../../includes/Defines.hpp"
#include "../RequestParser/RequestParser.hpp"
#include "../ConfigParser/ConfigParser.hpp"

// Classe Client responsável por guardar informações do cliente conectado,
// dentro do Servidor será um vector que manterá atualizado todos os clientes

class VirtualServer;
class RequestParser;
class Response;

class Client {
    public:
        Client();
        ~Client();
		Client(const Client& copy);
		Client& operator=(const Client& copy);


        std::string	getRequest(void);
        sockaddr_in	*getClientAddrPointer(void);
        socklen_t	*getClientAddrLenPointer(void);
        int			getClientSocket(void) const;
        ssize_t     getBytesRead(void) const;

        void        setClientSocket(int clientSocket);
        void        setClientAddrLen(socklen_t _client_addr_len);
        void        setBytesRead(ssize_t bytesRead);
        void        setRequest(std::string _requestBuffer);

		std::string uint8_to_string(const uint8_t* data, size_t size);
		void processRequest(const std::string& httpRequest, int *returnContentLenght, bool *requestComplete);
		int receiveRequest(size_t i, std::vector<pollfd> *pollFds);
		int sendResponse(size_t i, std::vector<pollfd> *pollFds, std::vector<VirtualServer>& Configs);

    private:
        int			_clientSocket;
        socklen_t	_client_addr_len;
        sockaddr_in	_client_addr;
        std::string	_request;
		std::string _stringBuffer;
        ssize_t		_bytesRead;
		uint8_t     _buffer[MAX_BUFFER_SIZE + 1];
};