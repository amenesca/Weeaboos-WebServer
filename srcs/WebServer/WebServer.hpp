/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/01 13:12:04 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "../ConfigParser/ConfigParser.hpp"
# include "../Socket/Socket.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class VirtualServer;
class WebServer {
    public:
        WebServer();
        ~WebServer();

        int configVServers(std::string configFilePath);
		int initConnection();
    private:
        ConfigParser configs;
		std::vector<VirtualServer> _vServers;
		Socket connection;

};

#endif
