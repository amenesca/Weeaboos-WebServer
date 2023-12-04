/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2023/12/04 13:42:01 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

int main(int argc, char **argv)
{
	try {
		WebServer server = WebServer();
		server.startServer(argc, argv);
	}
	catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
    }
    return (0);
}