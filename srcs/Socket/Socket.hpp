/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by femarque          #+#    #+#             */
/*   Updated: 2023/12/27 20:18:41 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <exception>
#include <sstream>
#include <iomanip>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/select.h>


#define MAX_BUFFER_SIZE 4096
#define PORT 18000
#define SA struct sockaddr
#define MAX_CLIENTS 92

class WebServer {
    private:
        int					_serverSocket; // File Descriptor dos Sockets
		int					_newClientSocket;
		int					_opt; // Opçao para o setsockopt
        int					_sendbyte;
        u_int8_t			_buffer[MAX_BUFFER_SIZE + 1];
        ssize_t				_bytesRead;
		ssize_t				_bytesSent;
        socklen_t			_server_addr_len, _client_addr_len;
		std::vector<int>	_clientSockets; // File Descriptor de varios clientes
		std::vector<struct pollfd>	_pollFds; // Vector de poll para observar 
		struct sockaddr_in	_server_addr, _client_addr;
    public:
        WebServer();
        ~WebServer();

        int		createSocket();
        int		setServerOptions();
        int		bindSocket();
        int		serverListen();
        int		acceptConnection();
        void	startServer();
        void    configAddress();
        void    handleRequest(int& i, int& clientSocket, int& nextClientId);
    
        class socketError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
        class setsockoptError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
        class bindError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
        class listenError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
        class acceptError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
        class readError : public std::exception {
        public:
            virtual const char* what() const throw();
        };
};

#endif
