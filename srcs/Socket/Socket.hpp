/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by femarque          #+#    #+#             */
/*   Updated: 2023/12/20 15:51:23 by femarque         ###   ########.fr       */
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
#include <sys/wait.h>
#include <arpa/inet.h>


#define MAX_BUFFER_SIZE 4096
#define PORT 18000
#define SA struct sockaddr

class WebServer {
    private:
        int _serversocket_fd, _clientsocket_fd;  // File Descriptor dos Sockets 
        int _opt; // Opçao para o setsockopt
        int _sendbyte;
        int _waitpid_status;
        char *_hexbin; //bin2hex return value that will be freed in the end
        pid_t _pid;
        u_int8_t _buffer[MAX_BUFFER_SIZE+1];
        u_int8_t _recbuffer[MAX_BUFFER_SIZE+1];
        ssize_t _valread;
        struct sockaddr_in _server_addr, _client_addr;
        socklen_t _server_addr_len, _client_addr_len;
    public:
    
        WebServer();
        ~WebServer();

        char *bin2hex(const unsigned char *input, size_t len);
        int startServer();
        int createSocket();
        int setServerOptions();
        int bindSocket();
        //int translateAddr(const char *addr);
        int serverListen();
        /*int serverAccept();
        int serverRead();*/
        int acceptConnection();
        void closeServer();
    
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
