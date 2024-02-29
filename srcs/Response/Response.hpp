#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../RequestParser/RequestParser.hpp"
# include "../VirtualServer/VirtualServer.hpp"
# include "../cgi/cgiHandler.hpp"
# include "../Client/Client.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class Response {
    private:
        char**          _envp;
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
       	Response(RequestParser request, VirtualServer virtualServerConfigs, char **envp, Client client);
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