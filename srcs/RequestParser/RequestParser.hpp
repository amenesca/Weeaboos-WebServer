#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>

class HttpParser {
	private:
		std::string _requestMethod;
		std::string _uri;
		std::string _httpVersion;
		std::map<std::string, std::string> _requestHeaders;

	public:
	// Constructors
		HttpParser();
		~HttpParser();
		HttpParser(std::string request);

	// Class Methods
		void	parse(std::string request);
		void	validateRequestLine();		
		void	validateHeader();
	
	// Getters
		std::string getMethod();
		std::string getUri();
		std::string getHttpVersion();
		std::map<std::string, std::string> getHeaders();
};

#endif
