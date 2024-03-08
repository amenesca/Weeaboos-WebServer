/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:35:28 by femarque          #+#    #+#             */
/*   Updated: 2024/03/07 17:25:17 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../Client/Client.hpp"

class cgiHandler {
    private:
        pid_t               _pid;
        std::vector<char*>  _env;
        int                 _request_pipe[2];
        RequestParser       _request;
        void antiBlock(int *pipe1, int *pipe2);
        bool writePipes(std::vector<std::string> body);
	public:
        cgiHandler();
        cgiHandler(RequestParser request);
        ~cgiHandler();

        std::vector<char*> createEnv(std::map<std::string, std::string> requestHeaders, Client client);
        std::string extrairQueryString(const std::string& uri);
        int getCgi();
        int postCgi(Client client);
		std::string getScriptFilename(const std::string& requestURI);
};

#endif