/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:24:57 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/22 18:49:17 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {
	_configFileFstream.close();
}

ConfigParser::ConfigParser(const ConfigParser& copy) {
	*this = copy;
}

ConfigParser& ConfigParser::operator=(const ConfigParser& copy) {
	if (this != &copy) {
		this->_configFilePath =  copy._configFilePath;
		this->_vServers = copy._vServers;
	}
	return *this;
}

std::vector<std::string> split(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

int ConfigParser::openConfig() {
	
	_configFileFstream.open(_configFilePath.c_str(), std::ios::in);

	if (!_configFileFstream.is_open()) {
		throw OpenError();
	}

	return (0);
}

void ConfigParser::treatLocation(VirtualServer* currentServer, std::string locationPath)
{
	std::string buff;
	Location location;
	location._locationPath = locationPath;
	bool rightBrace = false;

	while (std::getline(this->_configFileFstream >> std::ws, buff)) {
		if (buff.find("root", 0) != std::string::npos) {
			location._root = split(buff)[1];
		}
		else if (buff.find("cgi_extension", 0) != std::string::npos) {
			location._cgi_extension = split(buff)[1];
		}
		else if (buff.find("upload", 0) != std::string::npos) {
			location._upload = split(buff)[1];
		}
		else if (buff.find("index", 0) != std::string::npos) {
			location._index = split(buff);
		}
		else if (buff.find("autoindex", 0) != std::string::npos) {
			location._autoindex = split(buff)[1];
		}
		else if (buff.find("methods", 0) != std::string::npos) {
			location._methods = split(buff);
		}
		else if (buff.find("return", 0) != std::string::npos) {
			location._return = split(buff)[1];
		}
		else if (buff.find("}", 0) != std::string::npos) {
			rightBrace = true;
			break;
		}
		else {
			throw InvalidSyntax();
		}
	}
	if (rightBrace == false) {
			throw InvalidSyntax();
	}
	currentServer->getLocationAddress()->push_back(location);
} 

void ConfigParser::configServer(VirtualServer* currentServer)
{
	std::string buff;
	bool rightBrace = false;

	while (std::getline(this->_configFileFstream >> std::ws, buff)) {
		if (buff == "}") {
			rightBrace = true;
			break ;
		}
		else if (buff.find("error_page", 0) != std::string::npos) {
			currentServer->setErrorPage(split(buff));
		}
		else if (buff.find("listen", 0) != std::string::npos || buff.find("port", 0) != std::string::npos) {
			currentServer->setPort(strtod(split(buff)[1].c_str(), NULL));
		}
		else if (buff.find("server_name", 0) != std::string::npos) {
			currentServer->setServerName(split(buff)[1]);
		}
		else if (buff.find("body_size", 0) != std::string::npos) {
			currentServer->setBodySize(split(buff)[1]);
		}
		else if (buff.find("location", 0) != std::string::npos) 
		{
			if (buff.find("{", buff.size() - 1) == std::string::npos || split(buff).size() != 3)
			{
				throw InvalidSyntax();
			}
			treatLocation(currentServer, split(buff)[1]);
		}
		else {
			throw InvalidSyntax();
		}
	}
	if (rightBrace == false) {
			throw InvalidSyntax();
	}
}

void ConfigParser::setVServers()
{	
	std::string buff;
	size_t i = 0;

	while(std::getline(this->_configFileFstream >> std::ws, buff)) {
		std::istringstream iss(buff);
		std::string token;
		iss >> token;

		if (token == "server" && buff.find("{", buff.size() - 1) != std::string::npos && split(buff).size() == 2)
		{
			VirtualServer serverInstance;
			this->_vServers.push_back(serverInstance);
			configServer(&_vServers[i]);
			std::cout << "Virtual Server Adicionado" << std::endl;
			serverInstance.~VirtualServer();
			i++;
		}
		else
		{
			throw InvalidSyntax();
		}
	}
}

int ConfigParser::initConfig() {
	openConfig();
	setVServers();
	return (0);
}

void ConfigParser::setConfigFilePath(std::string configFilePath) {
	this->_configFilePath = configFilePath;
}

std::string ConfigParser::getConfigFilePath() const {
	return this->_configFilePath;
}

std::vector<VirtualServer> ConfigParser::getVServers() const {
	return (_vServers);
}

const char *ConfigParser::OpenError::what() const throw() {
	return ("WebServer: error: can't open configuration file.");
}

const char *ConfigParser::InvalidSyntax::what() const throw() {
	return ("WebServer: error: invalid syntax.");
}
