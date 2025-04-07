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

/** 
 * This function processes a client's request to part from one or more channels. The following checks and actions are performed:
 * 
 * 1. The list of channel names is parsed (comma-separated).
 * 2. For each channel:
 *    - Verifies the channel exists.
 *    - Checks if the client is currently in the channel.
 *    - If the client is an operator and the only operator in the channel, transfers operator privileges to another client (if possible).
 * 3. Sends a PART message to all other members of the channel, notifying them of the client's departure.
 * 4. Removes the client from the channel and updates the channel list.
 * 5. If the channel becomes empty after the client parts, it is removed from the server's list of channels.
 */

void Server::Part(Client &client, const std::string &channels, const std::string &message)
{
	std::stringstream ss(channels);
	std::string channelName;

	while (std::getline(ss, channelName, ','))
	{
		if (channelName[0] != '#')
			channelName = "#" + channelName;
		auto it = _channels.find(channelName);
		if (it == _channels.end())
		{
			sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
			continue;
		}

		Channel &channel = it->second;

		if (!channel.isClientInChannel(&client))
		{
			sendToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), channelName));
			continue;
		}

		if (channel.isOperator(&client) && channel.countOperators() == 1)
		{
			std::vector<Client *> clients = channel.getUsers();
			if (!clients.empty())
			{
				for (Client *potentialOp : clients)
				{
					if (potentialOp != &client)
					{
						channel.setOperator(potentialOp);
						channel.unsetOperator(&client);
						channel.displayChannelMessagePart(potentialOp, &client);
						sendToClient(*potentialOp, RPL_YOUREOPER(potentialOp->getNick()));
						break;
					}
				}
			}
		}

		for (Client *member : channel.getUsers())
		{
			sendToClient(*member, RPL_PART(client.getNick(), client.getUserName(), client.getHostName(), channelName, message));
		}

		channel.removeClient(&client);
		client.unsetJoinedChannel(&channel);

		if (channel.getUsers().empty())
			_channels.erase(it);
	}
}

