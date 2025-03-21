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
	std::string response = "Server received your message: " + message + "\n";
	if (send(client.getFd(), response.c_str(), response.size(), 0) == -1)
		perror("Error sending message to client.");
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

	std::vector<std::string> tokens = split(message);
	if (tokens.empty()) 
	{
		std::cerr << "Received an empty message" << std::endl;
		return;
	}

	if (client.getState() == REGISTERING)
	{
		std::cout << "[DEBUG] Client is registering..." << std::endl;
	
		if (tokens.size() < 2)
		{
			sendToClient(client, "461 PASS :Not enough parameters");
			return;
		}
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (tokens[i] == "CAP")
				Cap(client, tokens);
			else if (tokens[i] == "PASS" && i + 1 < tokens.size())
				Pass(client, tokens[i + 1]);
			else if (tokens[i] == "USER" && i + 4 < tokens.size())
				UserName(client,  tokens[i + 1],  tokens[i + 4]);
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
	if (client.getState() == REGISTERED)
	{
		std::cout << "[DEBUG] Client already registered." << std::endl;

		if (tokens[0] == "PING")
		{ 
			std::string arg;
			std::istringstream iss(message);
			iss >> arg;
			iss >> arg;
			sendToClient(client, "PONG " + arg);
		}

		else if (tokens[0] == "NICK" && tokens.size() > 1)
            Nick(client, tokens[1]);

		// commands["JOIN"] = [&]() { 
		// 	std::string channel, password;
		// 	std::istringstream iss(message);
		// 	iss >> channel; // JOIN
		// 	iss >> channel; // #channel
		// 	iss >> password; // password (optionnel)
		// 	Join(client, channel, password);
		// };

		// commands["PRIVMSG"] = [&]() { 
		// 	std::string target, msg;
		// 	std::istringstream iss(message);
		// 	iss >> target; // PRIVMSG
		// 	iss >> target; // Destinataire (#channel ou pseudo)
		// 	std::getline(iss, msg); // Message
		// 	Privmsg(client, target, msg);
		// };

		// commands["MODE"] = [&]() { 
		// 	std::string target, mode;
		// 	std::istringstream iss(message);
		// 	iss >> target; // MODE
		// 	iss >> target; // Target
		// 	std::getline(iss, mode); // Mode
		// 	Mode(client, target, mode);
		// };

		// commands["TOPIC"] = [&]() { 
		// 	std::string channel, topic;
		// 	std::istringstream iss(message);
		// 	iss >> channel; // TOPIC
		// 	iss >> channel; // Channel
		// 	std::getline(iss, topic); // Nouveau topic
		// 	Topic(client, channel, topic);
		// };

		// commands["KICK"] = [&]() { Kick(client, message); };
		// commands["INVITE"] = [&]() { Invite(client, message); };
		else if (tokens[0] == "QUIT")
			Quit(client, message);
	}
}
