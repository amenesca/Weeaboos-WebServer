/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:22:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/02/14 13:22:07 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "../ConfigParser/ConfigParser.hpp"
# include "../VirtualServer/VirtualServer.hpp"
# include "../Socket/Socket.hpp"
# include "../../includes/Defines.hpp"
# include "../../includes/Includes.hpp"

class VirtualServer;
class WebServer {
    public:
        WebServer();
        ~WebServer();

        int configVServers(std::string configFilePath);
		int initConnection(char **envp);
    private:
        ConfigParser configs;
		std::vector<VirtualServer> _vServers;
		Socket connection;

};

#endif
