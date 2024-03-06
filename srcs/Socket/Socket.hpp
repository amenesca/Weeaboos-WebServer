/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:22:02 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/01 15:56:35 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../ConfigParser/ConfigParser.hpp"
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
        Client						*_clients;
		int							_clientsCount;
        int							_serverSocket;
		int							_opt;
        uint8_t                     _buffer[MAX_BUFFER_SIZE + 1];
        socklen_t					_server_addr_len;
		std::vector<struct pollfd>	_pollFds;
		struct sockaddr_in			_server_addr;
		// ex webserver
		ConfigParser configs;
		// *************************
    public:
        Socket();
        ~Socket();

		void setVServers(std::vector<VirtualServer>& vServers);

		//  ex webserver
		int configVServers(std::string configFilePath);
		// ****************************

        int		createSocket();
        int		setServerOptions();
        int		bindSocket();
        int		serverListen();
        int		Connection();
        int     acceptConnection();
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