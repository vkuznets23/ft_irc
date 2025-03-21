/* **************************************************************************************** */
/*                                                                                          */
/*                                                        ::::::::::: :::::::::   ::::::::  */
/*                                                           :+:     :+:    :+: :+:    :+:  */
/*                                                          :+:     :+:    :+: :+:          */
/*                                                         :+:     :+:+:+:+:  :+:           */
/*  By: Viktoriia Kuznetsova<vkuznets@student.hive.fi>    :+:     :+:    :+: :+:            */
/*      Juliette Mouette<jmouette@student.hive.fi>,      :+:     :+:    :+: :+:    :+:      */
/*      									        ::::::::::: :::    :::  ::::::::        */
/*                                                                                          */
/* **************************************************************************************** */

#pragma once

#include "Client.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <csignal>

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
    void acceptNewConnection(int server_fd);
    void processPollResult(const std::vector<struct pollfd> &fds);
    void handleConnections(int server_fd);

    // handleMessage for commands like
    void handleClientMessage(Client &client, const std::string &message);

    // clean up
    void cleanupResources(int server_fd);

private:
    int _port;
    std::string _password;
    std::vector<Client *> _clients;
};