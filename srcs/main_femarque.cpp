/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2023/12/28 16:10:10 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Socket/Socket.hpp"

void signalHandler(int signum) {
    exit(signum);
}

int main(int argc, char **argv)
{
	signal(SIGINT, signalHandler);
	try {
		(void)argc;
		(void)argv;
		Socket server = Socket();
		server.startServer();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

  return (0);
}