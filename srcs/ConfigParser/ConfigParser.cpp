#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {

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

void ConfigParser::configServer(VirtualServer* currentServer) {
	std::string buff;
	while (std::getline(this->_configFileFstream, buff)) {
		if (buff.find("listen", 0) != std::string::npos) {
			currentServer->setPort(strtod(split(buff)[1].c_str(), NULL));
		}
		if (buff.find("server_name", 0) != std::string::npos) {
			currentServer->setServerName(split(buff)[1]);
		}
		if (buff.find("body_size", 0) != std::string::npos) {
			currentServer->setBodySize(split(buff)[1]);
		}
		if (buff.find("location", 0) != std::string::npos) {
			currentServer->setLocation(split(buff)[1]);
		}
		if (buff.find("root", 0) != std::string::npos) {
			currentServer->setRoot(split(buff)[1]);
		}
		if (buff.find("index", 0) != std::string::npos) {
			currentServer->setIndex(split(buff));
		}
	}
}

void ConfigParser::setVServers() {
	
	std::string buff;

	while(std::getline(this->_configFileFstream >> std::ws, buff)) {
		if (buff == "server {") {
			VirtualServer serverInstance;
			configServer(&serverInstance);
			this->_vServers.push_back(serverInstance);
			std::cout << "Virtual Server Adicionado" << std::endl;
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
    
	return ("WebServer: error: invalid syntax .");
}
