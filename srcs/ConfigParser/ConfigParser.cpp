#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {

}

ConfigParser::~ConfigParser() {

}

int ConfigParser::openReadEvaluate() {
	_configFileFstream.open(_configFilePath.c_str(), std::ios::in);

	if (!_configFileFstream.is_open()) {
		throw OpenError();
	}

	while (std::getline(_configFileFstream, _configLineRead)) {
		
	}

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