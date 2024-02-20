#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../includes/Classes.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class cgiHandler;

class Response {
    private:
        int         _status;
        std::string _response;
        std::string _body;
        std::string _header;
        RequestParser _request;
		std::string httpMessage;
		cgiHandler cgihandler;

    public:
        Response();
       	Response(int status, std::string response, std::string body, std::string header);
        Response(Response const &src);
        ~Response();

        Response &operator=(Response const &src);

        int         getStatus() const;
        std::string getResponse() const;
        std::string getHeader() const;

        void        setStatus(int status);
        void        setResponse(std::string response);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);

        void    send();
        void processFileForHTTPResponse(std::stringstream &file, std::string statusCode);
};

#endif