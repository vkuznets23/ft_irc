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

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
		return("");

	size_t last = str.find_last_not_of(" \t\r\n");
	return (str.substr(first, last - first + 1));
}

void Server::Invite(Client &client, const std::string &nickname, const std::string &channelName)
{

	Client *invite = findClient(nickname);
	
	if (!invite)
	{
		sendToClient(client, ERR_NOSUCHNICK(client.getNick(), nickname));
		return;
	}

	std::string cleanChannelName = trim(channelName);
	if (cleanChannelName[0] != '#')
		cleanChannelName = "#" + cleanChannelName;

	Channel *channel = getChannelByChannelName(cleanChannelName);

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

	if (channel->isClientInChannel(invite))
	{
		sendToClient(client, ERR_USERONCHANNEL(client.getNick(), nickname, channelName));
		return;
	}

	if (channel->getInviteOnlyState() && !channel->isOperator(&client))
	{
		sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
		return;
	}

	channel->addInvite(nickname);
	sendToClient(*invite, ":" + client.getNick() + " INVITE " + nickname + " " + cleanChannelName + "\r\n");
	sendToClient(client, RPL_INVITING(client.getNick(), nickname, cleanChannelName));
}
