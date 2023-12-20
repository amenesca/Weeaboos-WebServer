#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include <exception>
#include <cerrno>
#include <cstring>

class RequestParser {
	private:
		std::string _requestMethod;
		std::string _uri;
		std::string _httpVersion;
		std::map<std::string, std::string> _requestHeaders;

	// Private Methods
		int _validateUri();
	public:
	// Constructors
		RequestParser();
		~RequestParser();
		RequestParser(std::string request);

	// Class Methods
		void	parse(std::string request);
		void	validateRequestLine();		
		void	validateHeader();
	
	// Getters
		std::string getMethod();
		std::string getUri();
		std::string getHttpVersion();
		std::map<std::string, std::string> getHeaders();

	// Exceptions
		class invalidMethod : public std::exception {
        public:
            virtual const char* what() const throw();
        };

		class invalidHttpVersion : public std::exception {
        public:
            virtual const char* what() const throw();
        };
};

#endif
