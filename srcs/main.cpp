/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2024/02/14 13:22:01 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer/WebServer.hpp"

void signalHandler(int signum) {
    exit(signum);
}

int main(int argc, char **argv, char **envp)
{
	signal(SIGINT, signalHandler);
	try {
	WebServer server;
		if (argc == 2)
		{
			server.configVServers(argv[1]);
			server.initConnection(envp);
		} else if (argc == 1) {
			server.configVServers("./conf/default.conf");
			server.initConnection(envp);
		} else {
			std::cerr << "WebServer: error: Invalid arguments." << std::endl;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
  return (0);
}
