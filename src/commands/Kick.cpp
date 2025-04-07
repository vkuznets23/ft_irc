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

void Server::Kick(Client &client, const std::string &channelName, const std::string &target, std::string &reason)
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

	if(target == client.getNick())
	{
		sendToClient(client, ERR_CANNOTSENDTOCHAN(client.getNick(), channelName));
		return;
	}

	std::string cleanedReason = reason;
	if (!cleanedReason.empty() && cleanedReason[1] == ':')
	cleanedReason.erase(0, 2);
	cleanedReason.erase(std::find_if(cleanedReason.rbegin(), cleanedReason.rend(), 
				[](unsigned char ch) { return !std::isspace(ch); }).base(), cleanedReason.end());


	channel->displayChannelMessageKick(client, cleanedReason, target);
	sendToClient(client, RPL_KICK(client.getNick(), client.getUserName(), client.getHostName(), channelName, target, cleanedReason));
	sendToClient(*targetClient, RPL_KICK(client.getNick(), client.getUserName(), client.getHostName(), channelName, target, cleanedReason));
	
	channel->removeClient(targetClient);
}
