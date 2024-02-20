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
    _response(""),
    _body(""),
    _header("")
	{}

Response::Response(int status, std::string response, std::string body, std::string header) 
:   _status(status),
    _response(response),
    _body(body),
    _header(header)
    {}

Response::Response(Response const &src) {
    *this = src;
}

Response::~Response() {}

Response &Response::operator=(Response const &src) {
    if (this != &src) {
        this->_status = src._status;
        this->_response = src._response;
        this->_header = src._header;
    }
    return (*this);
}

int         Response::getStatus() const {
    return (_status);
}

std::string Response::getResponse() const {
    return (_response);
}

std::string Response::getHeader() const {
    return (_header);
}

void    Response::setStatus(int status) {
    _status = status;
}

void    Response::setResponse(std::string response) {
    _response = response;
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
   httpMessage = _header;
   httpMessage.append(_body);
}

void Response::processFileForHTTPResponse(std::stringstream &file, std::string statusCode) {
    std::string content = file.str();

    _body = content;
   setHeader(statusCode, "text/html");
}
