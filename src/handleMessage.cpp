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

#include "../inc/Server.hpp"

void Server::sendToClient(Client client, const std::string &message)
{
	std::string response = "Server received your message: " + message + "\r\n";
	if (send(client.getFd(), response.c_str(), response.size(), 0) == -1)
		perror("Error sending message to client.");
	else
		std::cout << "[DEBUG] Sent to client: " << message << std::endl;
}

std::vector<std::string> Server::split(const std::string &str)
{
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;

	while (iss >> token)
		tokens.push_back(token);

	return (tokens);
}

void Server::handleClientMessage(Client &client, const std::string &message)
{
	std::cout << "Received message: " << message << std::endl;

	if (client.getState() == REGISTERING)
	{
		std::cout << "[DEBUG] Client is registering..." << std::endl;

		std::vector<std::string> tokens = split(message);
		if (tokens.size() < 2)
		{
			sendToClient(client, "Error");
			return;
		}
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i] == "CAP")
				Cap(client, tokens);
			else if (tokens[i] == "PASS" && i + 1 < tokens.size())
				Pass(client, tokens[i + 1]);
			else if (tokens[i] == "USER" && i + 4 < tokens.size())
				UserName(client, tokens[i + 1], tokens[i + 4]);
			else if (tokens[i] == "NICK" && i + 1 < tokens.size())
				Nick(client, tokens[i + 1]);
		}
		if (client.getUserNameOK() && client.getNickOK() && client.getPasswdOK())
		{
			std::cout << "[DEBUG] All registration conditions met, transitioning to REGISTERED." << std::endl;
			client.setState(REGISTERED);
			sendToClient(client, "001 :Welcome to the IRC server, " + client.getUserName());
		}
	}
	else
	{
		std::cout << "[DEBUG] Client already registered." << std::endl;

		std::string arg[3];
		std::istringstream iss(message);
		iss >> arg[0];

		std::cout << "[DEBUG] Command received: " << arg[0] << std::endl;

		if (arg[0] == "PING")
		{
			iss >> arg[1];
			sendToClient(client, "PONG " + arg[1]);
		}
		else if (arg[0] == "NICK")
		{
			iss >> arg[1];
			Nick(client, arg[1]);
		}
		else if (arg[0] == "QUIT")
			Quit(client, message);
	}
}
