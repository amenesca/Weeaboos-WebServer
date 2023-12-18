#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>

class HttpParser {
	private:
		std::string method;
		std::string uri;
		std::string httpVersion;
		std::map<std::string, std::string> headers;

	public:
		HttpParser();
		~HttpParser();
		void parse(std::string request);

};

#endif
