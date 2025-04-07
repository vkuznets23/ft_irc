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
		sendToClient(client, ERR_CHANNELISFULL(client.getNick(), channelName));
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
 * This function allows a client to join one or more channels, optionally providing
 * passwords for password-protected channels. It processes the JOIN request as follows:
 * 
 * 1. Parses the comma-separated list of channel names and passwords.
 * 2. For each channel:
 *    - Validates the channel name format.
 *    - Checks if the channel already exists:
 *       a. If it exists, verifies access rights (invite-only, user limit, password).
 *       b. If validation passes, adds the client to the channel.
 *    - If the channel doesn't exist, creates it and assigns the client as operator.
 * 3. Sends the appropriate JOIN message to all channel members.
 * 4. Sends the channel topic (or "no topic" message) to the client.
 * 5. Sends the NAMES list for the channel to the client.
 */


void Server::Join(Client &client, std::string &channels, std::string &passwords)
{
	if (channels.empty())
	{
		sendToClient(client, "\r\n");
		return;
	}

	std::vector<std::string> channelList;
	std::stringstream ssChannels(channels);
	std::string channelName;
	while (std::getline(ssChannels, channelName, ','))
		channelList.push_back(channelName);

	std::vector<std::string> passwordList;
	std::stringstream ssPasswords(passwords);
	std::string pass;
	while (std::getline(ssPasswords, pass, ','))
		passwordList.push_back(pass);

	for (size_t i = 0; i < channelList.size(); ++i)
	{
		std::string chan = channelList[i];
		std::string pass = i < passwordList.size() ? passwordList[i] : "";

		if (!isChannelValid(client, chan))
			continue;

		auto it = _channels.find(chan);
		if (it != _channels.end())
		{
			Channel &channel = it->second;

			if (!checkChannelType(client, channel, chan, pass))
				continue;

			channel.addClient(client);
			client.setJoinedChannel(&channel);
			channel.removeInvite(client.getNick());

			for (Client *member : channel.getUsers())
				sendToClient(*member, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), chan));

			handleNamesCommand(client, chan);

			std::string topic = channel.getTopic();
			sendToClient(client, topic.empty() ? RPL_NOTOPIC(client.getNick(), chan) : RPL_TOPIC(client.getNick(), chan, topic));
		}
		else
		{
			// Create new channel
			Channel newChannel(chan);
			newChannel.setTimestamp();
			_channels[chan] = newChannel;

			Channel &createdChannel = _channels[chan];
			createdChannel.addClient(client);
			client.setJoinedChannel(&createdChannel);

			createdChannel.setOperator(&client);
			createdChannel.unsetInviteOnly();
			createdChannel.unsetUserLimit();

			if (pass.empty())
				createdChannel.unsetChannelPassword();
			else
				createdChannel.setChannelPassword(pass);

			sendToClient(client, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), chan));
			sendToClient(client, RPL_YOUREOPER(client.getNick()));
			handleNamesCommand(client, chan);
		}
	}
}
