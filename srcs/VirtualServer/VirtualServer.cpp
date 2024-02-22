/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:16 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:22:17 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./VirtualServer.hpp"

VirtualServer::VirtualServer() {}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer& copy) {
	*this = copy;
}

VirtualServer& VirtualServer::operator=(const VirtualServer& copy) {
	if (this != &copy) {
		this->_bodySize = copy._bodySize;
		this->_errorPage = copy._errorPage;
		this->_location = copy._location;
		this->_port = copy._port;
		this->_serverName = copy._serverName;
	}
	return *this;
}

void VirtualServer::setPort(int port) {
	this->_port = port;
}

void VirtualServer::setServerName(std::string serverName) {
	this->_serverName = serverName;
}

void VirtualServer::setBodySize(std::string bodySize) {
	this->_bodySize = bodySize;
}

void VirtualServer::setErrorPage(std::vector<std::string> errorPage) {
	this->_errorPage = errorPage;
}

// void VirtualServer::PushLocation(Location location) {
// 	this->_location.push_back(location);
// }

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

std::vector<std::string> VirtualServer::getErrorPage(void) const {
	return(_errorPage);
}