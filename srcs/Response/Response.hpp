#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../includes/Classes.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class cgiHandler;

class Response {
    private:
        int         _status;
        std::string _body;
        std::string _header;
		std::string _httpMessage;
        RequestParser _request;
		VirtualServer _virtualServerConfigs;
		cgiHandler cgihandler;

    public:
        Response();
       	Response(RequestParser request, VirtualServer virtualServerConfigs);
        ~Response();

        int         getStatus() const;
        std::string getHeader() const;
		std::string getHttpMessage() const;

        void        setStatus(int status);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);

        void    send();
        void processFileForHTTPResponse(std::stringstream &file, std::string statusCode);
};

#endif