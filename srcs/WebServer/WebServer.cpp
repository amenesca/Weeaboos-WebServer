#include "WebServer.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::configServer(std::string configFilePath) {
    this->configs.setConfigFilePath(configFilePath);
	this->configs.openReadEvaluate();
    return 0;
}