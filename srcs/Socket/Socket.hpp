/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/15 14:54:38 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKET_HPP
# define SOCKET_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../VirtualServer/VirtualServer.hpp"
# include "../Client/Client.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../Cgi/cgiHandler.hpp"
# include "../Response/Response.hpp"

class VirtualServer;
class Client;
class RequestParser;
class Socket {
    private:
		std::vector<VirtualServer>	_vServers;
        Client				        _client;
        int							_serverSocket;
		int							_newClientSocket;
		int							_opt;
        uint8_t                     _buffer[MAX_BUFFER_SIZE];
        std::string                 _requestBuffer;
        ssize_t						_bytesRead;
        socklen_t					_server_addr_len, _client_addr_len;
		std::vector<struct pollfd>	_pollFds;
		struct sockaddr_in			_server_addr, _client_addr;
    public:
        Socket();
        ~Socket();

		void setVServers(std::vector<VirtualServer> vServers);

        int		createSocket();
        int		setServerOptions();
        int		bindSocket();
        int		serverListen();
        int		Connection();
        int     acceptConnection();
        int     receiveRequest(int i);
        int     sendResponse(int i);
        void	configAddress();
        void	startServer();
        int const &getClientSocket();
        std::string uint8_to_string(const uint8_t* data, size_t size);
    
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