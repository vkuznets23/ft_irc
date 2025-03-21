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
#include <sstream>
#include <cstring>
#include <csignal>

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 999;

class Server
{
	private:
		int _port;
		std::string _password;
		std::vector<Client *> _clients;

	public:
		// constructors
		Server();
		Server(int port, std::string password);
		Server(const Server &o);
		Server &operator=(const Server &o);
		~Server();

		void runServer();

		// run server
		int  createServerSocket();
		void bindAndListen(int server_fd);
		void acceptNewConnection(int server_fd);
		void handleConnections(int server_fd);

		// handleMessage for commands like
		void sendToClient(Client client, const std::string &message);
		std::vector<std::string> split(const std::string &str);
		void handleClientMessage(Client &client, const std::string &message);

		// commands
		void Pass(Client &client, const std::string password);
		void UserName(Client &client, const std::string username, const std::string realname);
		bool isNickTaken(const std::string &nickname) const;
		void Nick(Client &client, const std::string nickname);
		void Cap(Client &client, const std::vector<std::string>& tokens);
		void Quit(Client &client, std::string message);

		// clean up
		void cleanupResources(int server_fd);
};