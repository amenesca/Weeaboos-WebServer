/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2024/03/01 13:12:27 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer/WebServer.hpp"

void signalHandler(int signum) {
    exit(signum);
}

int main(int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	try {
	WebServer server;
		if (argc == 2)
		{
			server.configVServers(argv[1]);
			server.initConnection();
		} else if (argc == 1) {
			server.configVServers("./conf/default.conf");
			server.initConnection();
		} else {
			std::cerr << "WebServer: error: Invalid arguments." << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
  return (0);
}
