#include "WebServer.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::configServer(std::string configFilePath) {
    this->configs.setConfigFilePath(configFilePath);
	this->configs.initConfig();
	_vservers = this->configs.getVServers();
//	std::cout << _vservers[0].getIndex()[0] << std::endl;
    return 0;
}