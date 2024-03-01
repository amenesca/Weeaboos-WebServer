/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:09 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/01 13:11:56 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer() {

}

WebServer::~WebServer() {

}

int WebServer::configVServers(std::string configFilePath) {
    this->configs.setConfigFilePath(configFilePath);
	this->configs.initConfig();
	_vServers = this->configs.getVServers();
    return 0;
}

int WebServer::initConnection() {
	connection.setVServers(_vServers);
	connection.startServer();
	return (0);
}