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
#include "../../inc/Channel.hpp"
#include "../../inc/Message.hpp"

/**
 * This function is called when a client quits the server. It:
 * 1. Retrieves the list of channels the client has joined.
 * 2. Notifies other members of these channels that the client has left.
 * 3. Removes the client from all joined channels.
 *
 * The function ensures that the channel's member list is not modified
 * while iterating over it by making a copy beforehand.
 */

void Server::Quit(Client &client)
{
	std::vector<Channel *> joinedChannels = client.getJoinedChannels();

	for (Channel *channel : joinedChannels)
	{
		std::vector<Client *> members = channel->getClients();

		for (Client *member : members)
		{
			if (member == &client)
			{
				channel->removeClient(&client);
			}
		}
	}

	std::cout << "Client " << client.getNick() << " removed from all channels." << std::endl;
}