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

#include "WebServer.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	try {
		WebServer server = WebServer();
		server.startServer();
	}
	catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}