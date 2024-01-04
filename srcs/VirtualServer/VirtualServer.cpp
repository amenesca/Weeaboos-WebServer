#include "./VirtualServer.hpp"

VirtualServer::VirtualServer() {}

VirtualServer::~VirtualServer() {}

void VirtualServer::setPort(int port) {
	this->_port = port;
}

void VirtualServer::setServerName(std::string serverName) {
	this->_serverName = serverName;
}

void VirtualServer::setBodySize(std::string bodySize) {
	this->_bodySize = bodySize;
}

void VirtualServer::setLocation(std::string location) {
	this->_location = location;
}

void VirtualServer::setRoot(std::string root) {
	this->_root = root;
}

void VirtualServer::setIndex(std::vector<std::string> index) {
	this->_index = index;
}

int VirtualServer::getPort(void) const {
	return(_port);
}

std::string VirtualServer::getServerName(void) const {
	return(_serverName);
}

std::string VirtualServer::getBodySize(void) const {
	return(_bodySize);
}

std::string VirtualServer::getLocation(void) const {
	return(_location);
}

std::string VirtualServer::getRoot(void) const {
	return(_root);
}

std::vector<std::string> VirtualServer::getIndex(void) const {
	return(_index);
}