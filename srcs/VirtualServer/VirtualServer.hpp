/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:14 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:22:15 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALSERVER_HPP
# define VIRTUALSERVER_HPP

#include "../../includes/Includes.hpp"
#include "../Utils/Utils.hpp"

struct Location {
	std::string _locationPath;
	std::string _root;
	std::string _cgi_extension;
	std::string _upload;
	std::string _autoindex;
	std::vector<std::string> _methods;
	std::string _return;
	std::vector<std::string> _index;
};

class VirtualServer {
	public:
		VirtualServer();
		~VirtualServer();

		void setPort(int port);
		void setServerName(std::string serverName);
		void setBodySize(std::string bodySize);
		void setErrorPage(std::vector<std::string>);

		int getPort(void) const;
		std::string getServerName(void) const;
		std::string getBodySize(void) const;
		std::vector<Location> getLocation(void) const;
		std::vector<Location>* getLocationAddress(void);
		std::vector<std::string> getErrorPage(void) const;
	private:
		int _port;
		std::string _serverName;
		std::string _bodySize;
		std::vector<std::string> _errorPage;
		std::vector<Location> _location;
};

#endif