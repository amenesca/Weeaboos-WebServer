/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2023/12/27 15:51:30 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

int main(int argc, char **argv)
{
	try {
		(void)argc;
		(void)argv;
		WebServer server = WebServer();
		server.startServer();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

  return (0);
}
