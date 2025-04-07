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
 * This function processes the client's request to quit the server, performing the following actions:
 * 
 * 1. Iterates over all channels the client is currently a member of, and removes the client from each channel using the PART command.
 * 2. Sends a QUIT message to all clients notifying them of the client's departure from the server.
 * 3. Closes the client's socket and disables the corresponding poll file descriptor.
 * 4. Removes the client from the server's list of clients.
 * 5. Frees the client's memory by deleting the client object.
 * 6. Logs the client's disconnection from the server to the console.
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

