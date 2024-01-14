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

int VirtualServer::getPort(void) const {
	return(_port);
}

std::string VirtualServer::getServerName(void) const {
	return(_serverName);
}

std::string VirtualServer::getBodySize(void) const {
	return(_bodySize);
}

std::vector<Location> VirtualServer::getLocation(void) const {
	return(_location);
}

std::vector<Location>* VirtualServer::getLocationAddress(void) {
	return(&_location);
}