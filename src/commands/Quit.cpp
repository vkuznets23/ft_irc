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

void Server::disablePollfd(int fd)
{
	auto it = find_if(_pollFds.begin(), _pollFds.end(),
					[fd](struct pollfd& pfd) { return pfd.fd == fd; });
	if (it != _pollFds.end())
	{
		it->fd = -1;
	}
}

void Server::Quit(Client &client, std::string message)
{
	std::vector<Channel *> joinedChannels = client.getJoinedChannels();
	const std::string nick = client.getNick();

	for (Channel *channel : joinedChannels)
	{
		Part(client, channel->getChannelName(), message);
	}

	sendToClient(client, RPL_QUIT(nick, client.getUserName(), client.getHostName(), message));

	disablePollfd(client.getFd());
	close(client.getFd());

	Client *clientToDelete = nullptr;
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == &client)
		{
			clientToDelete = *it;
			_clients.erase(it);
			break;
		}
	}

	if (clientToDelete)
	{
		delete clientToDelete;
		clientToDelete = nullptr;
	}

	std::cout << "Client " << nick << " removed from server." << std::endl;
}

