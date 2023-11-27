#include "WebServer.hpp"

namespace http
{
    WebServer::WebServer(std::string ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(), m_incomingMessage(), m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)) {
        startServer();
    }

    WebServer::~WebServer() {
        closeServer();
    }

    int WebServer::startServer() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (m_socket < 0)
        {
            throw socketError();
            return 1;
        }
        return 0;
    }

    void WebServer::closeServer()
    {
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }

    const char *WebServer::socketError::what() const throw() {
        std::string errorReturn =  "Error: socket(): " + std::string(strerror(errno));
        return (errorReturn.c_str()); 
}
} // namespace http
