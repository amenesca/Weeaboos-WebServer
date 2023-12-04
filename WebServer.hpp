/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by femarque          #+#    #+#             */
/*   Updated: 2023/12/04 13:48:47 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <exception>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>


#define MAX_BUFFER_SIZE 4096
#define PORT 80
#define SA struct sockaddr

class WebServer {
    private:
        int _socket_fd, _new_socket_fd; // File Descriptor dos Sockets 
        int _opt; // Opçao para o setsockopt
        int _sendbyte, _n;
        char _buffer[MAX_BUFFER_SIZE];
        char _recbuffer[MAX_BUFFER_SIZE];
        ssize_t _valread;
        struct sockaddr_in _socket_address;
        socklen_t _socket_address_len;
    public:
    
        WebServer();
        ~WebServer();
    
        int startServer(char **argv);
        int createSocket();
        int attachSocket();
        int bindSocket();
        int translateAddr(const char *addr);
        int serverListen();
        int serverAccept();
        int serverRead();
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