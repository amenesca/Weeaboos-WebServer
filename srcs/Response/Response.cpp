/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/03/01 13:08:32 by femarque         ###   ########.fr       */
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

Response::Response(RequestParser request, Client &client) 
:   _client(client),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage(""),
	_request(request)
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

std::string Response::readData(const std::string& uri, VirtualServer &vServer)
{
    std::string path;
    if (vServer._location.size() > 1) {
        for (size_t i = 0; i < vServer._location.size(); i++) {
            if (vServer._location[i]._locationPath == uri) {
                path = vServer._location[i]._root + vServer._location[i]._index[1];
//                std::cout << path << std::endl;
            }
        }
    }
    else {
        path = vServer._location[0]._root + uri;
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

void Response::handleGET(VirtualServer &vServer)
{
    std::string	uri = _request.getUri();
//    std::cout << "BODY: " << _request.getBody() << "\n"; // Desnecessário o GET não tem body na request!
    std::string	data = readData(uri, vServer);
     if (!data.empty()) {
        setStatus(200);
        _body = readData(uri, vServer);
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

		std::cout << "*** TESTANDO BODYDATA ***" << std::endl;
	for (size_t i = 0; i < bodyData.size(); i++) {
		std::cout << bodyData[i] << std::endl;
	}
	
    std::string uri = _request.getUri();
    std::cout << "PROCURANDO URI: " << uri << "\n";
	
    if (uri.substr(uri.length() - 3) == ".py") {
        std::cout << "DENTRO DO POST\n";
        cgiHandler post_cgi = cgiHandler();
        post_cgi.postCgi(_request, _client);
        setStatus(200);
        setHeader("200 OK", "text/plain");
        send();
    }
    else {
        std::cout << "DENTRO DO ELSE\n";
        if (_request.getBody()[0].empty()) {
            setStatus(400);
            setHeader("400 Bad Request", "text/plain");
            _body = "400 Bad Request: No request body found";
            return ;
        }

        setStatus(200);
        setHeader("200 OK", "text/plain");
        _body = "Received POST data:\n" + bodyData[0];
        send();
    }  
}

void Response::httpMethods(VirtualServer &vServer)
{
    if (_request.getMethod() == "GET") {
		handleGET(vServer);
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
