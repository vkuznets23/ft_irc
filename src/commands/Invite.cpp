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

void Server::Invite(Client &inviter, const std::string &nickname, const std::string &channelName)
{

	Client *invite = findClient(nickname);
	
	if (!invite)
	{
		sendToClient(inviter, ERR_NOSUCHNICK(inviter.getNick(), nickname));
		return;
	}

	Channel *channel = getChannelByChannelName(channelName);
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
	sendToClient(inviter, RPL_INVITING(inviter.getNick(), nickname, channelName));
	sendToClient(*invite, ":" + inviter.getNick() + " INVITE " + nickname + " " + channelName + "\r\n");
}
