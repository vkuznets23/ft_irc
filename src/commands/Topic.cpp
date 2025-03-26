
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

bool Channel::isOperator(Client *client) const
{
    return std::find(_operatorList.begin(), _operatorList.end(), client) != _operatorList.end();
}

/**
 * If the client provides a new topic, it is updated (if they have the necessary permissions).
 * If no topic is provided, the current topic of the channel is sent to the client.
 */

void Server::Topic(Client &client, const std::string &channelName, const std::string &newTopic)
{
	auto it = _channels.find(channelName);
	if (it == _channels.end())
	{
		std::cerr << "Channel " << channelName << " does not exist." << std::endl;
		return;
	}

	Channel &channel = it->second;

	if (newTopic.empty())
	{
		std::string topic = channel.getTopic().empty() ? "No topic set" : channel.getTopic();
		sendToClient(client, "Current topic for " + channelName + ": " + topic);
		return;
	}

	if (!channel.isClientInChannel(&client))
	{
		std::cerr << "Client " << client.getNick() << " is not in channel " << channelName << std::endl;
		return;
	}

	if (channel.isTopicRestricted() && !channel.isOperator(&client))
	{
		std::cerr << "Client " << client.getNick() << " does not have permission to change the topic in " << channelName << std::endl;
		return;
	}

	channel.setTopic(&client, newTopic);
	std::cout << "Topic for " << channelName << " changed to: " << newTopic << std::endl;

	for (Client *member : channel.getClients())
	{
		sendToClient(*member, "Topic for " + channelName + " changed to: " + newTopic);
	}
}
