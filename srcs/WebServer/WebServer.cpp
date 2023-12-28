#include "WebServer.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::configServer(std::string configFilePath) {
    this->configs.setConfig(configFilePath);

    return 0;
}