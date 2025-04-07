
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
 * This function processes a client's request to either retrieve or set the topic of a channel. The following actions are performed:
 * 
 * 1. Checks if the channel exists. If not, sends an error message (ERR_NOSUCHCHANNEL).
 * 2. If no new topic is provided, it sends the current topic of the channel to the client. 
 *    - If the channel has no topic set, it sends the RPL_NOTOPIC message.
 * 3. If a new topic is provided, the following checks are performed:
 *    - Verifies the client is a member of the channel. If not, an error message is sent (ERR_USERNOTINCHANNEL).
 *    - If the channel restricts topic changes to operators only, it checks if the client is an operator. If not, an error message is sent (ERR_CHANOPRIVSNEEDED).
 * 4. Cleans the new topic (removes leading and trailing spaces, and potential invalid characters).
 * 5. If the new topic is empty, it unsets the topic for the channel and notifies the client and other members.
 * 6. If the new topic is valid, it sets the topic for the channel and notifies the client and other members.
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
			sendToClient(client, RPL_NOTOPIC(client.getNick(), channelName));
		else
		{
			std::cout << "getTopic :'" << channel.getTopic() << "'" << std::endl;
			sendToClient(client, RPL_TOPIC(client.getNick(), channelName, channel.getTopic()));
		}
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

	std::string cleanedTopic = newTopic;
	if (!cleanedTopic.empty() && cleanedTopic[1] == ':')
		cleanedTopic.erase(0, 2);
	cleanedTopic.erase(std::find_if(cleanedTopic.rbegin(), cleanedTopic.rend(), 
				[](unsigned char ch) { return !std::isspace(ch); }).base(), cleanedTopic.end());
	if (!cleanedTopic.empty() && cleanedTopic[0] == ':')
		cleanedTopic.erase(0, 1);
	
	if (cleanedTopic.empty())
	{
		channel.unsetTopic();
		channel.displayChannelMessageTopic(client, cleanedTopic);
		sendToClient(client, RPL_NOTOPIC(client.getNick(), channelName));
	}
	else
	{
		channel.setTopic(cleanedTopic);
		channel.displayChannelMessageTopic(client, channel.getTopic());
		sendToClient(client, RPL_TOPIC(client.getNick(), channelName, channel.getTopic()));
	}
}

