#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {

}

ConfigParser::~ConfigParser() {

}

int ConfigParser::openConfig() {
	
	_configFileFstream.open(_configFilePath.c_str(), std::ios::in);

	if (!_configFileFstream.is_open()) {
		throw OpenError();
	}

	return (0);
}

int ConfigParser::initConfig() {

	openConfig();

	return (0);
}

void ConfigParser::setConfigFilePath(std::string configFilePath) {
	
	this->_configFilePath = configFilePath;
}

std::string ConfigParser::getConfigFilePath() const {
	
	return this->_configFilePath;
}

const char *ConfigParser::OpenError::what() const throw() {
    
	return ("WebServer: error: can't open configuration file.");
}

const char *ConfigParser::InvalidSyntax::what() const throw() {
    
	return ("WebServer: error: invalid syntax .");
}
