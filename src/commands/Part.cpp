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

void Server::Part(Client &client, const std::string &channelName, std::string message)
{
	auto it = _channels.find(channelName);
	if (it == _channels.end())
	{
		sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}

	Channel &channel = it->second;

	if (!channel.isClientInChannel(&client))
	{
		sendToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), channelName));
		return;
	}

	for (Client *member : channel.getClients())
	{
		sendToClient(*member, RPL_PART(client.getNick(), client.getUserName(), client.getHostName(), channelName, message));
	}

	channel.removeClient(&client);

	if (channel.getOperator() == &client)
	{
		std::vector<Client *> clients = channel.getClients();
		if (!clients.empty())
		{
			channel.setOperator(clients.front());
			sendToClient(*clients.front(), RPL_YOUREOPER(clients.front()->getNick(), channelName));
		}
		else
		{
			_channels.erase(it);
			return;
		}
	}
}
