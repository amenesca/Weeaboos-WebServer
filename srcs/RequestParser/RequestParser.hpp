/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:05 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/05 14:32:53 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include <exception>
# include <cerrno>
# include <cstring>
# include <vector>

class RequestParser {
	private:
		std::string _requestMethod;
		std::string _uri;
		std::string _httpVersion;
		std::string _portNumber;
		std::string _queryString;
		std::vector<std::string> _requestBody;
		std::map<std::string, std::string> _requestHeaders;

	// Private Methods
		int _validateUri();
	public:
	// Constructors
		RequestParser();
		~RequestParser();
		RequestParser(std::string request);
		RequestParser(const RequestParser& copy);
		RequestParser& operator=(const RequestParser& copy);

	// Class Methods
		void	parse(std::string request);
		void	validateRequestLine();		
		void	validateHeader();
	
	// Getters
		std::string getMethod() const ;
		std::string getUri() const;
		std::string getHttpVersion() const;
		std::vector<std::string> getBody() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getPortNumber() const;
		std::string getQueryString();

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
