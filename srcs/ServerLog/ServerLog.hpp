/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLog.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:57:09 by femarque          #+#    #+#             */
/*   Updated: 2024/03/15 15:01:07 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOG_HPP
# define SERVERLOG_HPP

# include "../../includes/Includes.hpp"
# include "../../includes/Defines.hpp"
# include "../RequestParser/RequestParser.hpp"
# include "../Client/Client.hpp"

class ServerLog {
    private:
        int _stdout_fd;
        int _stderr_fd;
    public:
        ServerLog();
        ~ServerLog();

        void createLog();
};

#endif