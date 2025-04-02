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

#include "../../inc/Server.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Message.hpp"

Client* Server::findClient(const std::string &nick)
{
	for (Client *client : _clients)
	{
		if (client->getNick() == nick)
		{
			return (client);
		}
	}
	return (nullptr);
}

bool Server::checkBasicErrors(Client &client, const std::string &msgtarget, const std::string &message)
{
	if (message.empty())
	{
		sendToClient(client, ERR_NOTEXTTOSEND(client.getNick()));
		return (false);
	}
	if (msgtarget.empty())
	{
		sendToClient(client, ERR_NORECIPIENT(client.getNick(), "PRIVMSG"));
		return (false);
	}
	
	return (true);
}

std::vector<std::string> Server::parseTargets(const std::string &msgtarget, Client &client)
{
	std::istringstream targetStream(msgtarget);
	std::string target;
	std::vector<std::string> allTargets;
	std::set<std::string> usedTargets;
	int targetCount = 0;

	while (std::getline(targetStream, target, ','))
	{
		if (target.empty()) continue;

		targetCount++;
		if (targetCount > 15)
		{
			sendToClient(client, ERR_TOOMANYTARGETS(client.getNick(), msgtarget));
			return {};
		}

		if (!usedTargets.insert(target).second)
		{
			sendToClient(client, ERR_TOOMANYTARGETS(client.getNick(), "Duplicate recipients. No message delivered"));
			return {};
		}
		allTargets.push_back(target);
	}

	return allTargets;
}


void Server::Privmsg(Client &client, const std::string &msgtarget, const std::string &message)
{
	if (!checkBasicErrors(client, msgtarget, message))
		return;

	std::vector<std::string> allTargets = parseTargets(msgtarget, client);

	if (allTargets.empty())
		return;

	for (const auto &target : allTargets)
	{
		if (target[0] == '#')
		{
			auto channelIt = _channels.find(target);
			if (channelIt == _channels.end())
			{
				sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), target));
			}
			else
			{
				Channel &channel = channelIt->second;
				if (!channel.isClientInChannel(&client))
					sendToClient(client, ERR_CANNOTSENDTOCHAN(client.getNick(), target));
				else
					channel.displayChannelMessage(client, message);
			}
		}
		else
		{
			Client *recipient = findClient(target);
			std::string cleanedMessage = message;
			cleanedMessage.erase(std::find_if(cleanedMessage.rbegin(), cleanedMessage.rend(), 
							[](unsigned char ch) { return !std::isspace(ch); }).base(), cleanedMessage.end());
		
			if (recipient)
				sendToClient(*recipient, RPL_PRIVMSG(client.getNick(), target, cleanedMessage));
			else
				sendToClient(client, ERR_NOSUCHNICK(client.getNick(), target));
		}
	}
}
