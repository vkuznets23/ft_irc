/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktoria <viktoria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:37:19 by viktoria          #+#    #+#             */
/*   Updated: 2025/03/19 11:10:55 by viktoria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {}

Server::Server(int port, std::string password) : _port(port), _password(password) {}

Server::Server(const Server &o)
{
    this->_port = o._port;
    this->_password = o._password;
}

Server &Server::operator=(const Server &o)
{
    if (this != &o)
    {
        this->_port = o._port;
        this->_password = o._password;
    }
    return *this;
}

Server::~Server() {}