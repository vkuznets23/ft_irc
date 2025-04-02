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

bool Server::isChannelValid(Client &client, const std::string& channel)
{
	bool	valid = true;

	if (channel.empty() || (channel[0] != '#' && channel[0] != '+' && channel[0] != '!' && channel[0] != '&'))
		valid = false;

	if (channel.size() > 50)
		valid = false;
	
	if (channel[0] == '!')
	{
		if (channel.size() != 6 || channel.substr(1, 5).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != std::string::npos)
			valid = false;
	}

	for (size_t i = 1; i < channel.size(); ++i)
	{
		char c = channel[i];
		if (c == '\0' || c == '\a' || c == '\r' || c == '\n' || c == ' ' || c == ',' || c == ':')
			valid = false;
	}
	if (valid == false)
		sendToClient(client, ERR_BADCHANNELNAME(client.getNick(), channel));
	return (true);
}

bool Server::checkChannelType(Client &client, Channel &channel, const std::string &channelName, const std::string &password)
{
	if (channel.getInviteOnlyState() && !channel.isInvited(client.getNick()))
	{
		sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
		return (false);
	}

	if (channel.getUserLimit() != -1 && channel.getUserCount() >= channel.getUserLimit() && !channel.isOperator(&client))
	{
		sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
		return (false);
	}

	if (!password.empty() && !channel.getChannelPassword().empty() && channel.getChannelPassword() != password)
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
	(void) password;
	if (channels.empty())
	{
		sendToClient(client, "\r\n");
		return;
	}

	std::stringstream ss(channels);
	std::string channelName;

	while (std::getline(ss, channelName, ','))
	{
		if (!isChannelValid(client, channelName))
			return;

		auto it = _channels.find(channelName);
		if (it != _channels.end())
		{
			Channel &channel = it->second;

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
			return;
		}

		// Create a new channel if it does not exist
		Channel newChannel(channelName);
		newChannel.setTimestamp();
		_channels[channelName] = newChannel;

		Channel &createdChannel = _channels[channelName];
		createdChannel.addClient(client);
		createdChannel.setOperator(&client);
		createdChannel.unsetInviteOnly();
		createdChannel.unsetUserLimit();

		if (password.empty())
			createdChannel.unsetChannelPassword();
		else
			createdChannel.setChannelPassword(password);

		sendToClient(client, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), channelName));
		sendToClient(client, RPL_NAMREPLY(client.getNick(), channelName));
		sendToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
		return;
	}
}
