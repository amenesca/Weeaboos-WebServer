/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2023/12/06 12:21:24 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer/WebServer.hpp"

int main(int argc, char **argv)
{
	WebServer server;
	if (argc == 2)
	{
		server.configServer(argv[1]);
	} else if (argc == 1) {
		server.configServer("../conf/default.conf");
	} else {
		std::cerr << "WebServer: error: Invalid arguments." << std::endl;
	}

  return (0);
}
