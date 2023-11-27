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

namespace http {
    class WebServer {
        private:
            std::string m_ip_address, m_serverMessage;
            int m_port, m_socket, m_new_socket;
            long m_incomingMessage;
            struct sockaddr_in m_socketAddress;
            socklen_t m_socketAddress_len;
        public:
        // Constructors
            WebServer(std::string ip_address, int port);
            ~WebServer();
        // Methods
            int startServer();
            void closeServer();
        // Exceptions
            class socketError : public std::exception
            {
                public:
			        virtual const char* what() const throw();
            };
    };
} // namespace http

#endif