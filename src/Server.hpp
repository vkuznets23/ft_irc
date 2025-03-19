/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktoria <viktoria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:37:25 by viktoria          #+#    #+#             */
/*   Updated: 2025/03/19 12:35:31 by viktoria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

#include <iostream>
#include <vector>
#include <string>

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 999;

class Server
{
public:
    // constructors
    Server();
    Server(int port, std::string password);
    Server(const Server &o);
    Server &operator=(const Server &o);
    ~Server();

    void runServer();

    // run server
    int createServerSocket();
    void bindAndListen(int server_fd);
    void acceptConnection(int server_fd);
    void handleConnections(int server_fd);

    // handleMessage for commands like
    void handleClientMessage(Client &client, const std::string &message);

private:
    int _port;
    std::string _password;
    std::vector<Client *> _clients;
};