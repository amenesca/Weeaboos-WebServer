/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/02/14 13:23:55 by femarque         ###   ########.fr       */
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
