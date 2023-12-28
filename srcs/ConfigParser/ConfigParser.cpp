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
//	int vserver_quantity;
	size_t pos_http;
//	bool inside_http;
//	bool inside_server;
	bool found_http;
//	bool found_server;

//	vserver_quantity = 0;
	pos_http = std::string::npos;
	found_http = false;
//	inside_http = false;
//	inside_server = false;

	openConfig();

	while (std::getline(_configFileFstream, _configLineRead)) {
		_configLineRead = trim(_configLineRead);

		if (_configLineRead.empty() || _configLineRead[0] == '#') {
            continue;
        }

		pos_http = _configLineRead.find_first_of("http", 4);
		if (pos_http != std::string::npos) {
			found_http = true;
		}
		if (found_http == true)
			std::cout << "Found http" << std::endl;


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