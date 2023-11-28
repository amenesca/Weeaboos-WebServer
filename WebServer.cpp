#include "WebServer.hpp"

namespace http
{
    WebServer::WebServer() {
    }
// ------------------------------------------------------------------- //
    WebServer::~WebServer() {
        closeServer();
    }
// ------------------------------------------------------------------- //
    int WebServer::startServer () {
        try {
            createSocket();
            attachSocket();
            bindSocket();
            serverListen();
            serverAccept();
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
// ------------------------------------------------------------------- //
    int WebServer::createSocket() {
        _socket_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (_socket_fd < 0)
        {
            throw socketError();
            return 1;
        }
        return 0;
    }
// ------------------------------------------------------------------- //
    int WebServer::attachSocket() {
        int setsockopt_return = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt));
        
        if (setsockopt_return < 0)
        {
            throw setsockoptError();
            return 1;
        }
        return 0;
    }
// ------------------------------------------------------------------- //
    int WebServer::bindSocket () {
        _socket_address.sin_family = AF_INET;
        _socket_address.sin_addr.s_addr = INADDR_ANY;
        _socket_address.sin_port = htons(PORT);

        int bind_return = bind(_socket_fd, (struct sockaddr*)&_socket_address, _socket_address_len);

        if (bind_return < 0)
        {
            throw bindError();
            return 1;
        }
        return 0;
    }
// ------------------------------------------------------------------- //
    int WebServer::serverListen() {
        int listen_return = listen(_socket_fd, 3);
        
        if (listen_return < 0)
        {
            throw listenError();
            return 1;
        }
        return 0;
    }
// ------------------------------------------------------------------- //
    int WebServer::serverAccept() {
        int accept_return = accept(_socket_fd, (struct sockaddr*)&_socket_address,
                  &_socket_address_len);
        
        if (accept_return < 0)
        {
            throw acceptError();
            return 1;
        }
        return 0;
    }
// ------------------------------------------------------------------- //
    int WebServer::serverRead() {
        // fazer
    }
// ------------------------------------------------------------------- //
    void WebServer::closeServer()
    {
        close(_socket_fd);
        close(_new_socket_fd);
        exit(0);
    }
// ------------------------------------------------------------------- //
    const char *WebServer::socketError::what() const throw() {
        std::string errorReturn =  "Error: socket(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
// ------------------------------------------------------------------- //
    const char *WebServer::setsockoptError::what() const throw() {
        std::string errorReturn =  "Error: setsockopt(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
// ------------------------------------------------------------------- //

    const char *WebServer::bindError::what() const throw() {
        std::string errorReturn =  "Error: bind(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
// ------------------------------------------------------------------- //

    const char *WebServer::listenError::what() const throw() {
        std::string errorReturn =  "Error: listen(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
// ------------------------------------------------------------------- //

    const char *WebServer::acceptError::what() const throw() {
        std::string errorReturn =  "Error: accept(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
// ------------------------------------------------------------------- //
    const char *WebServer::readError::what() const throw() {
        std::string errorReturn =  "Error: read(): " + std::string(strerror(errno));
        return (errorReturn.c_str());
    }
} // namespace http
