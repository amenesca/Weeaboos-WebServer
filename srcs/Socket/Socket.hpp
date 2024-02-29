/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/29 15:32:15 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef SOCKET_HPP
# define SOCKET_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../VirtualServer/VirtualServer.hpp"
# include "../Client/Client.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../cgi/cgiHandler.hpp"
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
        char				        _buffer[65000 + 1];
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
        int		Connection(char **envp);
        int     acceptConnection();
        int     receiveRequest(size_t *i);
        int     sendResponse(size_t *i, char **envp);
        void	configAddress();
        void	startServer(char **envp);
        int const &getClientSocket();
    
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