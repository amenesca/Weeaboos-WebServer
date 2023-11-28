/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:21:50 by femarque          #+#    #+#             */
/*   Updated: 2023/11/28 18:23:45 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./WebServer.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
    WebServer server = WebServer();
    server.startServer();
    return (0);
}