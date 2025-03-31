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
#include <unistd.h>

/**
 * The Quit function handles the QUIT command from a client.
 * When a client sends a QUIT message, it can include a quit message,
 * and the client will be removed from the server's client list.
 * If no message is provided, the default message "Client left" is sent.
 * The client's channels are cleaned up, and the client is disconnected from the server.
 */

void Server::Quit(Client &client, std::string message)
{
	if (client.getState() == DISCONNECTED)
	{
		std::cerr << "Client " << client.getNick() << " is already disconnected." << std::endl;
		return;
	}

	if (message.empty())
	{
		message = "Client has disconnected.";
	}

	for (Channel *channel : client.getJoinedChannels())
	{
		for (Client *member : channel->getClients())
		{
			if (member != &client)
			{
				sendToClient(*member, client.getNick() + " has left the channel: " + message);
			}
		}
		channel->removeClient(&client);
	}

	close(client.getFd());
	std::cout << "Client " << client.getNick() << " has disconnected." << std::endl;

	client.setState(DISCONNECTED);

	auto it = std::find(_clients.begin(), _clients.end(), &client);
	if (it != _clients.end())
		_clients.erase(it);

	sendToClient(client, "Goodbye " + client.getNick() + "!");
}
