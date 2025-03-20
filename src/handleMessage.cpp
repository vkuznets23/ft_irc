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
#include <unordered_map>
#include <functional>

void Server::sendToClient(Client client, const std::string &message)
{
	std::string response = "Server received your message: " + message;
	if (send(client.getFd(), response.c_str(), response.size(), 0) == -1)
		perror("Error sending message to client.");
}

std::vector<std::string> Server::split(const std::string &str)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);

	while (std::getline(tokenStream, token, ' '))
	{
		if (!token.empty())
			tokens.push_back(token);
	}
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

	std::unordered_map<std::string, std::function<void()>> commands;

	if (client.getState() == REGISTERING)
	{
		std::cout << "[DEBUG] Client is registering..." << std::endl;
	
		if (tokens.size() < 2)
		{
			sendToClient(client, "461 PASS :Not enough parameters");
			return;
		}
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			//if (tokens[i] == "CAP")
			//	commands["CAP"] = [&]() { Cap(client, tokens, i); };
			if (tokens[i] == "PASS")
				commands["PASS"] = [&]() { Pass(client, tokens, i); };
			else if (tokens[i] == "USER")
				commands["USER"] = [&]() { UserName(client, tokens, i); };
			else if (tokens[i] == "NICK" && i + 1 < tokens.size())
				commands["NICK"] = [&]() { Nick(client, tokens[i + 1]); };
		}
		client.setState(REGISTERED);
	}	
	else
	{
		std::cout << "[DEBUG] Client already registered." << std::endl;

		commands["PING"] = [&]()
		{ 
			std::string arg;
			std::istringstream iss(message);
			iss >> arg;
			iss >> arg;
			sendToClient(client, "PONG " + arg);
		};

		commands["NICK"] = [&]()
		{ 
			std::string nickname;
			std::istringstream iss(message);
			iss >> nickname; // NICK
			iss >> nickname; // Nouveau pseudo
			Nick(client, nickname);
		};

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
		// commands["QUIT"] = [&]() { Quit(client, message); };
	}

	auto it = commands.find(tokens[0]);
	if (it != commands.end()) 
	{
		it->second();
	}
	else 
	{
		std::cerr << "[DEBUG] Unknown command: " << tokens[0] << std::endl;
	}
}
