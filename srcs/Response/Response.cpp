/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/03/07 17:18:06 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
:   _client(Client()),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage("")
	{}

Response::Response(RequestParser request, VirtualServer virtualServerConfigs, Client client) 
:   _client(client),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage(""),
	_request(request),
	_virtualServerConfigs(virtualServerConfigs)
    {}

Response::~Response() {}

int         Response::getStatus() const {
    return (_status);
}

std::string Response::getHeader() const {
    return (_header);
}

std::string Response::getHttpMessage() const {
	return (_httpMessage);
}

void    Response::setStatus(int status) {
    _status = status;
}

std::string Response::toString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

std::string    Response::setHeader(std::string status, std::string contentType) {
    std::string header;

    header.append("HTTP/1.1 ").append(status).append("\r\n");
    header.append("Content-Type: ").append(contentType).append("\r\n");
    header.append("Content-Length: ").append(toString(_body.size()));
    header.append("\r\n").append("\r\n");

    _header = header;
    return(header);
}

void Response::send() {
   _httpMessage = _header;
   _httpMessage.append(_body);
}

std::string Response::readData(const std::string& uri)
{
    std::string path;
    if (_virtualServerConfigs.getLocation().size() > 1) {
        for (size_t i = 0; i < _virtualServerConfigs.getLocation().size(); i++) {
            if (_virtualServerConfigs.getLocation()[i]._locationPath == uri) {
                path = _virtualServerConfigs.getLocation()[i]._root + _virtualServerConfigs.getLocation()[i]._index[1];
//                std::cout << path << std::endl;
            }
        }
    }
    else {
        path = _virtualServerConfigs.getLocation()[0]._root + uri;
	}
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        return ("");
    }
    std::string data;
    std::string line;
    while (std::getline(file, line)) {
        data += line;
        data += "\n";
    }
    file.close();
    return (data);
}

void Response::handleGET()
{
    std::string	uri = _request.getUri();
//    std::cout << "BODY: " << _request.getBody() << "\n"; // Desnecessário o GET não tem body na request!
    std::string	data = readData(uri);
     if (!data.empty()) {
        setStatus(200);
        _body = readData(uri);
        setHeader("200 OK", "text/html");
        send();
    }
    else
    {
        setStatus(404);
        setHeader("404 Not Found", "text/html");
        _body = "404 Not Found";
    }
}

void Response::handlePOST()
{
    std::vector<std::string> bodyData = _request.getBody();
    for (size_t i = 0; i < bodyData.size(); i++) {
        std::cout << "BODY: " << bodyData[i] << "\n";
    }
	
    std::string uri = _request.getUri();
    std::cout << "PROCURANDO URI: " << uri << "\n";
	
    if (_request.getBody().empty()) {
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
        _body = "400 Bad Request: No request body found";
        return ;
    }
    
    cgiHandler post_cgi = cgiHandler(_request);
    std::cout << "ANTES DO CGIHANDLER\n";
    post_cgi.postCgi(_client);
    setStatus(200);
    send();
}

void Response::httpMethods()
{
    if (_request.getMethod() == "GET") {
		handleGET();
	}
    else if (_request.getMethod() == "POST") {
		std::cout << "HANDLE POST" << std::endl;
        handlePOST();
    }
    else {
        setStatus(405); // 405 =  Method Not Allowed
        setHeader("405 Method Not Allowed", "text/plain");
        _body = "405 Method Not Allowed";
    }
    return ;
}
