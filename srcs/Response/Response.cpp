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

std::string Response::readData(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return ("");

    std::string data;
    std::string line;
    while (std::getline(file, line))
    {
        data += line;
        data += "\n";
    }

    file.close();

    return (data);
}

void Response::httpMethods(RequestParser& request)
{
    if (request.getMethod() == "GET")
        handleGET(request);
    else if (request.getMethod() == "POST")
        handlePOST(request);
    /*else if (request.getMethod() == "DELETE")
        handleDELETE(request);*/
    else
        setStatus(405); // 405 = Method Not Allowed
        setHeader("405 Method Not Allowed", "text/plain");
        _body = "405 Method Not Allowed";
        return ;
}

void Response::handleGET(RequestParser request)
{
    std::string	uri = request.getUri();
    std::string	data = readData(uri);
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

void Response::handlePOST(RequestParser request)
{
    std::string uri = request.getUri();
    if (request.getBody().empty())
	{
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
        _body = "400 Bad Request: No request body found";
        return ;
    }

    std::string bodyData = request.getBody();

    setStatus(200);
    setHeader("200 OK", "text/plain");
    _body = "Received POST data:\n" + bodyData;
}
