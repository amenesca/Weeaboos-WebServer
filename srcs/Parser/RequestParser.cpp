#include "./RequestParser.hpp"
#include <sstream>

HttpParser::HttpParser() {}

HttpParser::~HttpParser() {}

void	HttpParser::parse(std::string request) {

	std::string requestLine, headerLine;
	std::istringstream requestStream(request);


	std::getline(requestStream, requestLine);
	// Solicitation Line parsing
	std::istringstream requestLineStream(requestLine);
	requestLineStream >> this->method >> this->uri >> this->httpVersion;

	while(std::getline(requestStream, headerLine) && headerLine != "\r") {
		// Header parsing
		size_t separator = headerLine.find(":");
		if (separator != std::string::npos) {
			std::string key = headerLine.substr(0, separator);
			std::string value = headerLine.substr(separator + 2); // + 2 para ignorar ": " 
			this->headers[key] = value;
		}
	}
	return;
}
