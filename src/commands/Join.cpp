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
		std::cout << client->getNick() << " created and joined the channel " << channelName << std::endl;
	}
	else
	{
		Channel &channel = it->second;

		if (!channel.getChannelPassword().empty() && channel.getChannelPassword() != password)
		{
			std::cerr << "Incorrect password for joining " << channelName << std::endl;
			return;
		}

		if (channel.getUserLimit() > 0 && channel.getUserLimit() >= channel.getUserLimit())
		{
			std::cerr << "The channel " << channelName << " is full." << std::endl;
			return;
		}

		channel.addClient(client);
		client->joinChannel(&channel);
		std::cout << client->getNick() << " joined the channel " << channelName << std::endl;
	}
}
