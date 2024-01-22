#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include "../../includes/Classes.hpp"
# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"


class Response {
    private:
        int         _status;
        std::string _response;
        std::string _body;
        std::string _header;
        std::string _path;
        std::string _method;
        cgiHandler  _cgiHandler;

    public:
        Response();
        Response(int status, std::string response, std::string body, std::string header, std::string path, std::string method, cgiHandler cgiHandler);
        Response(Response const &src);
        ~Response();

        Response &operator=(Response const &src);

        int         getStatus() const;
        std::string getResponse() const;
        std::string getBody() const;
        std::string getHeader() const;
        std::string getPath() const;
        std::string getMethod() const;

        void        setStatus(int status);
        void        setResponse(std::string response);
        void        setPath(std::string path);
        void        setMethod(std::string method);

        std::string setHeader(std::string status, std::string contentType);
        void    send();
        void processFileForHTTPResponse(std::stringstream &file, std::string statusCode)
};

#endif