/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by femarque          #+#    #+#             */
/*   Updated: 2023/12/28 16:18:28 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../../includes/Includes.hpp"

#define MAX_BUFFER_SIZE 4096
#define PORT 18000
#define SA struct sockaddr
#define MAX_CLIENTS 92

class Socket {
    private:
        int							_serverSocket;
		int							_newClientSocket;
		int							_opt;
        u_int8_t					_buffer[MAX_BUFFER_SIZE + 1];
        ssize_t						_bytesRead;
		ssize_t						_bytesSent;
        socklen_t					_server_addr_len, _client_addr_len;
		std::vector<struct pollfd>	_pollFds;
		struct sockaddr_in			_server_addr, _client_addr;
    public:
        Socket();
        ~Socket();

        int		createSocket();
        int		setServerOptions();
        int		bindSocket();
        int		serverListen();
        int		acceptConnection();
        void	configAddress();
        void	startServer();
    
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