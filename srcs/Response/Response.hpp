#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../ConfigParser/ConfigParser.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../cgi/cgiHandler.hpp"
# include "../Client/Client.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class Response {
    private:
		Client			_client;
        int				_status;
        std::string		_body;
        std::string		_header;
		std::string		_httpMessage;
        RequestParser	_request;
		cgiHandler		cgihandler;
    public:
        Response();
       	Response(RequestParser request, Client &client);
        ~Response();

        int         getStatus() const;
        std::string getHeader() const;
		std::string getHttpMessage() const;

        void        setStatus(int status);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);
		std::string	readData(const std::string& uri, VirtualServer &vServer);

        void    send();
        void    httpMethods(VirtualServer &vServer);
		
		void handleGET(VirtualServer &vServer);
		void handlePOST();
};

#endif