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

void Client::joinChannel(Channel *channel)
{
    if (channel == nullptr)
        return;

    if (std::find(_joinedChannels.begin(), _joinedChannels.end(), channel) == _joinedChannels.end())
    {
        _joinedChannels.push_back(channel);
    }
}

/**
 * If the channel does not exist, it is created and the client is added.
 * If the channel exists, checks are performed:
 *  - If a password is required, it must be correct.
 *  - If the channel has a user limit, it must not be exceeded.
 * If all conditions are met, the client is added to the channel.
 */

void Server::Join(Client *client, const std::string &channelName, const std::string &password)
{
	auto it = _channels.find(channelName);
	if (it == _channels.end())
	{
		Channel newChannel(channelName);
		newChannel.addClient(client);
		_channels[channelName] = newChannel;
		client->joinChannel(&newChannel);
		sendToClient(*client, RPL_JOIN(client->getNick(), channelName));
	}
	else
	{
		Channel &channel = it->second;

		if (!channel.getChannelPassword().empty() && channel.getChannelPassword() != password)
		{
			sendToClient(*client, ERR_BADCHANNELKEY(client->getNick(), channelName));
			return;
		}

		if (channel.getUserLimit() > 0 && channel.getUserLimit() >= channel.getUserLimit())
		{
			sendToClient(*client, ERR_CHANNELISFULL(client->getNick(), channelName));
			return;
		}

		channel.addClient(client);
		client->joinChannel(&channel);
	}
}
