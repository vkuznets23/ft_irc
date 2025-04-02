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

#include "../inc/Message.hpp"
#include "../inc/Server.hpp"
#include <functional>
#include <map>
#include <sstream>

void Server::sendToClient(Client client, const std::string &message)
{
	std::string response = message + "\r\n";
	if (send(client.getFd(), response.c_str(), response.size(), 0) == -1)
		perror("Error sending message to client.");
	else
		std::cout << ">> " << message << std::endl;
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
    std::cout << std::endl << "Received message: " << message;

    if (client.getState() == REGISTERING)
    {
        std::cout << "Client is registering..." << std::endl;

		std::vector<std::string> tokens = split(message);
		if (tokens.size() < 2)
		{
			sendToClient(client, "Error");
			return;
		}
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i] == "PASS" && i + 1 < tokens.size())
				Pass(client, tokens[i + 1]);
			else if (tokens[i] == "USER" && i + 4 < tokens.size())
				UserName(client, tokens[i + 1], tokens[i + 4]);
			else if (tokens[i] == "NICK" && i + 1 < tokens.size())
				Nick(client, tokens[i + 1]);
		}
		if (client.getUserNameOK() && client.getNickOK() && client.getPasswdOK())
		{
			client.setState(REGISTERED);
			sendToClient(client, RPL_WELCOME(client.getNick(), client.getUserName(), client.getHostName()));
			sendToClient(client, RPL_AVAILABLECMD(client.getNick()));
		}
	}
	else
	{

        std::string arg[3];
        std::istringstream iss(message);
        iss >> arg[0];

		std::map<std::string, std::function<void(Client&, std::istringstream&) >> registeredCommands =
		{
			{"PING", [](Client &c, std::istringstream &iss)
			{
				std::string arg1;
				iss >> arg1;
				sendToClient(c, "PONG " + arg1);
			}},
			{"NICK", [this](Client &c, std::istringstream &iss)
			{
				std::string arg1;
				iss >> arg1;
				Nick(c, arg1);
			}},
			{"JOIN", [this](Client &c, std::istringstream &iss)
			{
				std::string channelName, password;
				iss >> channelName >> password;
				Join(c, channelName, password);
			}},
			{"TOPIC", [this](Client &c, std::istringstream &iss)
			{
				std::string channelName, newTopic;
				iss >> channelName;
				std::getline(iss, newTopic);
				Topic(c, channelName, newTopic);
			}},
			{"PART", [this, &message](Client &c, std::istringstream &iss)
			{
				std::string arg1;
				iss >> arg1;
				Part(c, arg1, message);
			}},
			{"PRIVMSG", [this](Client &c, std::istringstream &iss)
			{
				std::string nickname, message;
				iss >> nickname;
				std::getline(iss, message);
				Privmsg(c, nickname, message);
			}},
			{"INVITE", [this](Client &c, std::istringstream &iss)
			{
				std::string msgtarget, channelName;
				iss >> msgtarget;
				std::getline(iss, channelName);
				Invite(c, msgtarget, channelName);
			}},
			{"MODE", [this](Client &c, std::istringstream &iss)
			{
				std::string channelName;
				std::string modeMessage;
				iss >> channelName;
				std::getline(iss, modeMessage);
				handleMode(c, channelName, modeMessage);
			}},
			{"NAMES", [this](Client &c, std::istringstream &iss)
			{
				std::string channelName;
				iss >> channelName;

				handleNamesCommand(c, channelName);
			}},
			{"KICK", [this](Client &c, std::istringstream &iss)
			{
				std::string channelName, target, reason;
				iss >> channelName >> target;
				std::getline(iss, reason);
				if (!reason.empty() && reason[0] == ' ')
					reason.erase(0, 1);
				Kick(c, channelName, target, reason);
			}},
			{"QUIT", [this, &message](Client &c, std::istringstream &iss)
			{
				(void)iss;
				Quit(c, message);
			}}
		};

        auto it = registeredCommands.find(arg[0]);
        if (it != registeredCommands.end())
        {
            it->second(client, iss);
        }
    }
}
