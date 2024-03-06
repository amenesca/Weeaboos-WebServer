#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../../includes/Includes.hpp"
#include "../../includes/Defines.hpp"
#include "../RequestParser/RequestParser.hpp"
#include "../ConfigParser/ConfigParser.hpp"
#include "../Client/Client.hpp"
#include "../cgi/cgiHandler.hpp"

class Client;

class Response {
    private:
		Client			_client;
        int				_status;
        std::string		_body;
        std::string		_header;
		std::string		_httpMessage;
        RequestParser	_request;
		VirtualServer	_virtualServerConfigs;
		cgiHandler		cgihandler;
    public:
        Response();
       	Response(RequestParser request, VirtualServer virtualServerConfigs, Client client);
        ~Response();

        int         getStatus() const;
        std::string getHeader() const;
		std::string getHttpMessage() const;

        void        setStatus(int status);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);
		std::string	readData(const std::string& uri);

        void    send();
        void    httpMethods();
		
		void handleGET();
		void handlePOST();
};

#endif