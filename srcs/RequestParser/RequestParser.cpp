/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:07 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/29 14:41:11 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./RequestParser.hpp"
#include <sstream>

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

RequestParser::RequestParser(std::string request) {
	this->parse(request);
}

RequestParser::RequestParser(const RequestParser& copy) {
	*this = copy;
}

RequestParser& RequestParser::operator=(const RequestParser& copy) {
	if (this != &copy) {
		this->_httpVersion = copy._httpVersion;
		this->_requestBody = copy._requestBody;
		this->_requestHeaders = copy._requestHeaders;
		this->_requestMethod = copy._requestMethod;
		this->_uri = copy._uri;
	}
	return *this;
}

void RequestParser::parse(std::string request) {
    // Declaring variables
//	std::cout << "--- PARSER DA REQUEST ---" << std::endl;
	
    std::string requestLine, headerLine, bodyLine;
    std::istringstream requestStream(request);

    // Solicitation Line parsing
    std::getline(requestStream, requestLine);
    std::istringstream requestLineStream(requestLine);
    requestLineStream >> this->_requestMethod >> this->_uri >> this->_httpVersion;

    // Header parsing
    while(std::getline(requestStream, headerLine) && headerLine != "\r") {
        size_t separator = headerLine.find(": ");
        if (separator != std::string::npos) {
            std::string key = headerLine.substr(0, separator);
            std::string value = headerLine.substr(separator + 2); // + 2 para ignorar ": "

            // Se a chave for "Host", extrair apenas o conteúdo antes do ":"
            if (key == "Host") {
                size_t portSeparator = value.find(":");
                if (portSeparator != std::string::npos) {
                    value = value.substr(0, portSeparator);
                }
            }

            this->_requestHeaders[key] = value;
        }
    }
    // Body parsing
	if (_requestMethod == "POST")
	{
		while (std::getline(requestStream, bodyLine)) {
			_requestBody.push_back(bodyLine);
		}
	}
	
    return;
}

int RequestParser::_validateUri() {
    // Verificar se a URI começa com uma barra "/"
    if (_uri.empty() || _uri[0] != '/') {
        return -1; // Deu merda
    }

    // Verificar se os caracteres da URI são válidos
    for (std::size_t i = 1; i < _uri.length(); ++i) {
        char ch = _uri[i];

        if (!std::isalnum(ch) && ch != '/' && ch != '_' && ch != '-') {
            return -1; // Deu merda    
        }
    }

    return 0;
}

void RequestParser::validateRequestLine() {
	if (_requestMethod != "GET" && _requestMethod != "POST" && _requestMethod != "DELETE") {
		throw invalidMethod();
	}

	if (_httpVersion != "HTTP/1.1") {
		throw invalidHttpVersion();
	}

	_validateUri();
}

std::string RequestParser::getMethod() const {
	return _requestMethod;
}

std::string RequestParser::getUri() const {
	return _uri;
}

std::string RequestParser::getHttpVersion() const {
	return _httpVersion;
}

std::vector<std::string> RequestParser::getBody() const {
	return _requestBody;
}

std::map<std::string, std::string> RequestParser::getHeaders() const {
	return _requestHeaders;
}

const char *RequestParser::invalidMethod::what() const throw() {
	return "Error: invalid method in request";
}

const char *RequestParser::invalidHttpVersion::what() const throw() {
	return "Error: invalid http version";
}
