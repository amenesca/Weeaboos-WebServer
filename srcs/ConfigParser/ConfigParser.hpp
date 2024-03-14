/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:14 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:24:15 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_CPP
#define CONFIGPARSER_CPP

#include "../../includes/Includes.hpp"
#include "../Utils/Utils.hpp"

struct Location {
	std::string _locationPath;
	std::string _root;
	std::string _cgi_extension;
	std::string _upload;
	std::string _autoindex;
	std::vector<std::string> _methods;
	std::string _return;
	std::vector<std::string> _index;
};

struct VirtualServer {
	int _port;
		std::string _serverName;
		std::string _bodySize;
		std::vector<std::string> _errorPage;
		std::vector<Location> _location;
};

class ConfigParser {
    public:
		ConfigParser();
		~ConfigParser();

		ConfigParser(const ConfigParser& copy);
		ConfigParser& operator=(const ConfigParser& copy);

		int openConfig();
		int initConfig();
		void setVServers();
		void configServer(VirtualServer* currentServer);
		void treatLocation(VirtualServer* currentServer, std::string locationPath);
		std::vector<VirtualServer> getVServers(void) const;

		void setConfigFilePath(std::string configFilePath);
		std::string getConfigFilePath() const;

		class OpenError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidSyntax : public std::exception {
			public:
				virtual const char* what() const throw();
		};

	private:
		
		std::string		_configFilePath;
		std::ifstream	_configFileFstream;
		std::vector<VirtualServer> _vServers;
};

std::vector<std::string> split(const std::string& input);

#endif
