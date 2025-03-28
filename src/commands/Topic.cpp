
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
 * This function allows a client to view or modify the topic of a channel.
 * - If the channel does not exist, an error is returned.
 * - If the client is not a member of the channel, they cannot change the topic.
 * - If the topic is restricted and the client is not an operator, they cannot modify it.
 * - If no new topic is provided, the current topic is returned.
 * - If a new topic is provided, it is set and broadcasted to all members of the channel.
 */

void Server::Topic(Client &client, const std::string &channelName, const std::string &newTopic)
{
	auto it = _channels.find(channelName);
	if (it == _channels.end())
	{
		sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}

	Channel &channel = it->second;

	if (newTopic.empty())
	{
		if (channel.getTopic().empty())
			sendToClient(client, RPL_NOTOPIC(channelName));
		else
			sendToClient(client, RPL_TOPIC(channelName, channel.getTopic()));
		return;
	}

	if (!channel.isClientInChannel(&client))
	{
		sendToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), channelName));
		return;
	}

	if (channel.isTopicRestricted() && !channel.isOperator(&client))
	{
		sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
		return;
	}

	channel.setTopic(&client, newTopic);

	for (Client *member : channel.getClients())
	{
		sendToClient(*member, RPL_NEWTOPIC(channelName, channel.getTopic()));
	}
}
