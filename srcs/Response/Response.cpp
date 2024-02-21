/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/02/21 18:52:43 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
:   _status(0),
    _body(""),
    _header(""),
	_httpMessage("")
	{}

Response::Response(RequestParser request, VirtualServer virtualServerConfigs) 
:   _status(0),
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
    std::string str = ss.str();
    return str;
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

void Response::processFileForHTTPResponse(std::stringstream &file, std::string statusCode) {
    std::string content = file.str();

    _body = content;
   setHeader(statusCode, "text/html");
}

std::string Response::readData(const std::string& uri)
{
	std::string path;
	std::cout << "AQUI3" << std::endl;
	if (_virtualServerConfigs.getLocation()[0]._locationPath == uri) {
		path = _virtualServerConfigs.getLocation()[0]._root + _virtualServerConfigs.getLocation()[0]._index[0];
		std::cout << path << std::endl;
	}
	std::cout << "AQUI4" << std::endl;
	
    // std::ifstream file(path.c_str());
    // if (!file.is_open())
    //     return ("");

    // std::string data;
    // std::string line;
    // while (std::getline(file, line))
    // {
    //     data += line;
    //     data += "\n";
    // }

    // file.close();

     return ("");
}

void Response::httpMethods()
{
    if (_request.getMethod() == "GET") {
        
		std::cout << "AQUI1" << std::endl;
		
		handleGET();

		std::cout << "AQUI6" << std::endl;

	}
    // else if (_request.getMethod() == "POST")
    //     handlePOST();
    // /*else if (_request.getMethod() == "DELETE")
    //     handleDELETE(request);*/
    // else {
    //     setStatus(405); // 405 = Method Not Allowed
    //     setHeader("405 Method Not Allowed", "text/plain");
    //     _body = "405 Method Not Allowed";
	// }
    return ;
}

void Response::handleGET()
{
    std::string	uri = _request.getUri();
	std::cout << "AQUI2" << std::endl;
    std::string	data = readData(uri);
	std::cout << "AQUI5" << std::endl;
     if (!data.empty())
     {
        setStatus(200);
        setHeader("200 OK", "text/html");
        _body = data;
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
    std::string uri = _request.getUri();
    if (_request.getBody().empty())
	{
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
        _body = "400 Bad Request: No request body found";
        return ;
    }

    std::string bodyData = _request.getBody();

    setStatus(200);
    setHeader("200 OK", "text/plain");
    _body = "Received POST data:\n" + bodyData;
}
