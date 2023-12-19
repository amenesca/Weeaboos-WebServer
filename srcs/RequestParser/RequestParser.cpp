#include "./RequestParser.hpp"
#include <sstream>

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}

HttpParser::HttpParser(std::string request) {
	this->parse(request);
}

void	HttpParser::parse(std::string request) {

	std::string requestLine, headerLine;
	std::istringstream requestStream(request);

	std::getline(requestStream, requestLine);
	// Solicitation Line parsing
	std::istringstream requestLineStream(requestLine);
	requestLineStream >> this->_requestMethod >> this->_uri >> this->_httpVersion;

	while(std::getline(requestStream, headerLine) && headerLine != "\r") {
		// Header parsing
		size_t separator = headerLine.find(":");
		if (separator != std::string::npos) {
			std::string key = headerLine.substr(0, separator);
			std::string value = headerLine.substr(separator + 2); // + 2 para ignorar ": " 
			this->_requestHeaders[key] = value;
		}
	}
	return;
}

void HttpParser::validateRequestLine() {
	
}

std::string HttpParser::getMethod() {
	return _requestMethod;
}

std::string HttpParser::getUri() {
	return _uri;
}

std::string HttpParser::getHttpVersion() {
	return _httpVersion;
}

std::map<std::string, std::string> HttpParser::getHeaders() {
	return _requestHeaders;
}
