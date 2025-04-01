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

bool Server::checkChannelType(Client &client, Channel &channel, const std::string &channelName, const std::string &password)
{
	if (channel.getChannelType() == INVITE_ONLY && !channel.isOperator(&client))
	{
		sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
		return (false);
	}

	if (channel.getChannelType() == PRIVATE && !channel.isClientInChannel(&client))
	{
		sendToClient(client, ERR_BANNEDFROMCHAN(client.getNick(), channelName));
		return (false);
	}

	if (channel.getChannelType() == MODERATED && !channel.isOperator(&client))
	{
		sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
		return (false);
	}

	if (!channel.getChannelPassword().empty() && channel.getChannelPassword() != password)
	{
		sendToClient(client, ERR_BADCHANNELKEY(client.getNick(), channelName));
		return (false);
	}

	return (true);
}


/**
 * This function handles a client joining one or more channels. 
 * It checks if the channels exist, validates mode restrictions, adds the client to the channel, 
 * and sends appropriate messages to the client and other members of the channel.
 */

void Server::Join(Client &client, std::string &channels, std::string &password)
{
	if (channels.empty())
	{
		sendToClient(client, "\r\n");
		return;
	}

	std::stringstream ss(channels);
	std::string channelName;

	while (std::getline(ss, channelName, ','))
	{
		auto it = _channels.find(channelName);
		if (it != _channels.end())
		{
			Channel &channel = it->second;

			if (channel.getInviteOnlyState() && !channel.isInvited(client.getNick()))
			{
				sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
				continue;
			}

			if (!checkChannelType(client, channel, channelName, password))
				return;

			channel.addClient(client);
			channel.removeInvite(client.getNick());

			for (Client *member : channel.getClients())
			{
				sendToClient(*member, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), channelName));
				sendToClient(*member, RPL_NAMREPLY(client.getNick(), channelName));
				sendToClient(*member, RPL_ENDOFNAMES(client.getNick(), channelName));
			}

			std::string topic = channel.getTopic();
			sendToClient(client, topic.empty() ? RPL_NOTOPIC(channelName) : RPL_TOPIC(channelName, topic));
			sendToClient(client, RPL_AVAILABLECMD(client.getNick()));
			return;
		}

		// Create a new channel if it does not exist
		Channel newChannel(channelName);
		newChannel.setTimestamp();
		newChannel.setChannelType(PUBLIC);

		_channels[channelName] = newChannel;
		Channel &createdChannel = _channels[channelName];
		createdChannel.addClient(client);
		createdChannel.setOperator(&client);

		sendToClient(client, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), channelName));
		sendToClient(client, RPL_NAMREPLY(client.getNick(), channelName));
		sendToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
		return;
	}
}
