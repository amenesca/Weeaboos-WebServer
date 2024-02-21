/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:09 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:22:10 by amenesca         ###   ########.fr       */
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

int WebServer::initConnection(char **envp) {
	connection.setVServers(_vServers);
	connection.startServer(envp);
	return (0);
}