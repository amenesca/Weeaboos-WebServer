#include "WebServer.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::configVServers(std::string configFilePath) {
    this->configs.setConfigFilePath(configFilePath);
	this->configs.initConfig();
	_vServers = this->configs.getVServers();
	std::cout << _vServers[0].getLocation()[0]._root << std::endl;
    return 0;
}

int WebServer::initConnection(char **envp) {
	connection.setVServers(_vServers);
	connection.startServer(envp);
	return (0);
}