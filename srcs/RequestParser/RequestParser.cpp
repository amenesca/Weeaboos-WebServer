#include "./RequestParser.hpp"
#include <sstream>

RequestParser::RequestParser() {}

RequestParser::~RequestParser() {}

RequestParser::RequestParser(std::string request) {
	this->parse(request);
}

void	RequestParser::parse(std::string request) {
	// Declaring variables
	std::string requestLine, headerLine;
	std::istringstream requestStream(request);

	// Solicitation Line parsing
	std::getline(requestStream, requestLine);
	std::istringstream requestLineStream(requestLine);
	requestLineStream >> this->_requestMethod >> this->_uri >> this->_httpVersion;

	// Header parsing
	while(std::getline(requestStream, headerLine) && headerLine != "\r") {
		size_t separator = headerLine.find(":");
		if (separator != std::string::npos) {
			std::string key = headerLine.substr(0, separator);
			std::string value = headerLine.substr(separator + 2); // + 2 para ignorar ": " 
			this->_requestHeaders[key] = value;
		}
	}
	return ;
}

void RequestParser::_validateUri () {
	
}

void RequestParser::validateRequestLine() {
	if (_requestMethod != "GET" || _requestMethod != "POST" || _requestMethod != "DELETE") {
		throw invalidMethod();
	}

	if (_httpVersion != "HTTP/1.1") {
		throw invalidHttpVersion();
	}

	_validateUri();
}

std::string RequestParser::getMethod() {
	return _requestMethod;
}

std::string RequestParser::getUri() {
	return _uri;
}

std::string RequestParser::getHttpVersion() {
	return _httpVersion;
}

std::map<std::string, std::string> RequestParser::getHeaders() {
	return _requestHeaders;
}

const char *RequestParser::invalidMethod::what() const throw() {
    std::string errorReturn =  "Error: invalid method in request";
    return (errorReturn.c_str());
}

const char *RequestParser::invalidHttpVersion::what() const throw() {
    std::string errorReturn =  "Error: invalid http version";
    return (errorReturn.c_str());
}