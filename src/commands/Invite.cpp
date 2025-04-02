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

void Server::Invite(Client &inviter, const std::string &nickname, const std::string &channelName)
{

	Client *invite = findClient(nickname);
	
	if (!invite)
	{
		sendToClient(inviter, ERR_NOSUCHNICK(inviter.getNick(), nickname));
		return;
	}

	std::string cleanChannelName = trim(channelName);
	if (cleanChannelName[0] != '#')
		cleanChannelName = "#" + cleanChannelName;

	Channel *channel = getChannelByChannelName(cleanChannelName);

	if (!channel)
	{
		sendToClient(inviter, ERR_NOSUCHCHANNEL(inviter.getNick(), channelName));
		return;
	}

	if (!channel->isClientInChannel(&inviter))
	{
		sendToClient(inviter, ERR_NOTONCHANNEL(inviter.getNick(), channelName));
		return;
	}

	if (channel->isClientInChannel(invite))
	{
		sendToClient(inviter, ERR_USERONCHANNEL(inviter.getNick(), nickname, channelName));
		return;
	}

	if (channel->getInviteOnlyState() && !channel->isOperator(&inviter))
	{
		sendToClient(inviter, ERR_CHANOPRIVSNEEDED(inviter.getNick(), channelName));
		return;
	}

	channel->addInvite(nickname);
	sendToClient(*invite, ":" + inviter.getNick() + " INVITE " + nickname + " " + cleanChannelName + "\r\n");
	sendToClient(inviter, RPL_INVITING(inviter.getNick(), nickname, cleanChannelName));
}
