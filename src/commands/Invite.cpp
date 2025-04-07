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
 * This function allows a client to invite another user to a channel.
 * It performs various checks before sending the invitation:
 * 
 * 1. Checks if the invited nickname exists.
 * 2. Ensures the channel name is valid and exists.
 * 3. Confirms the inviter is a member of the channel.
 * 4. Verifies that the invited user is not already in the channel.
 * 5. If the channel is invite-only, checks if the inviter has operator privileges.
 * 
 * If all conditions are met, the invited user is added to the channel's invite list,
 * and both the inviter and invitee are notified.
 */

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
		sendToClient(client, ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));
		return;
	}

	if (channel->isClientInChannel(invite))
	{
		sendToClient(client, ERR_USERONCHANNEL(client.getNick(), nickname, channel->getChannelName()));
		return;
	}

	if (channel->getInviteOnlyState() && !channel->isOperator(&client))
	{
		sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channel->getChannelName()));
		return;
	}

	channel->addInvite(nickname);
	sendToClient(*invite, ":" + client.getNick() + " INVITE " + nickname + " " + channel->getChannelName() + "\r\n");
	sendToClient(client, RPL_INVITING(client.getNick(), nickname, channel->getChannelName()));
}
