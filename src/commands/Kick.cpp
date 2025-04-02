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

void Server::Kick(Client &client, const std::string &channelName, const std::string &target, const std::string &reason)
{
	Channel *channel = getChannelByChannelName(channelName);
	if (!channel)
	{
		sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}

	if (!channel->isClientInChannel(&client))
	{
		sendToClient(client, ERR_NOTONCHANNEL(client.getNick(), channelName));
		return;
	}

	if (!channel->isOperator(&client))
	{
		sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
		return;
	}

	Client *targetClient = findClient(target);
	if (!targetClient || !channel->isClientInChannel(targetClient))
	{
		sendToClient(client, ERR_USERNOTINCHANNEL(target, channelName));
		return;
	}

	std::string kickMsg = ":" + client.getNick() + " KICK " + channelName + " " + target + " :" + reason;
	for (Client *member : channel->getUsers())
	{
		sendToClient(*member, kickMsg);
	}

	channel->removeClient(targetClient);
}
