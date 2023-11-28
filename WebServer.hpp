#ifndef WEBSERVER_HPP
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
#include <arpa/inet.h>
#define PORT 8080

class WebServer {
    private:
        int _socket_fd, _new_socket_fd; // File Descriptor dos Sockets 
        int _opt; // Opçao para o setsockopt
        char _buffer[4096];
        ssize_t _valread;
        struct sockaddr_in _socket_address;
        socklen_t _socket_address_len;
    public:
    // Constructors
        WebServer();
        ~WebServer();
    // Methods
        int startServer();
        int createSocket();
        int attachSocket();
        int bindSocket();
        int serverListen();
        int serverAccept();
        int serverRead();
        void closeServer();
    // Exceptions
        class socketError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class setsockoptError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class bindError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class listenError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class acceptError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class readError : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};


#endif
