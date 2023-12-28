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

int ConfigParser::openReadEvaluate() {
	size_t pos_http;
	bool inside_http;
	bool inside_server;

	pos_http = std::string::npos;
	inside_http = false;
	inside_server = false;

	openConfig();

	while (std::getline(_configFileFstream, _configLineRead)) {
		_configLineRead = trim(_configLineRead);

		if (_configLineRead.empty() || _configLineRead[0] == '#') {
            continue;
        }

		pos_http = _configLineRead.find_first_of("http", 4);


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