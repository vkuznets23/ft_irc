#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"
#include <unordered_set>

// add to Client.cpp and Server.cpp
Client *Server::getClientByNickname(const std::string &nickname)
{
    auto it = std::find_if(_clients.begin(), _clients.end(),
                           [&nickname](Client *client) { return client->getNick() == nickname; });

    return (it != _clients.end()) ? *it : nullptr;
}

bool Server::userIsMemberOfChannel(Client &client, const std::string &channelName)
{
    Channel *channel = getChannelByChannelName(channelName);
    if (!channel)
        return false;

    return channel->isClientInChannel(&client);
}

Channel *Server::getChannelByChannelName(const std::string &channelName)
{
	auto it = _channels.find(channelName);

	if (it != _channels.end())
		return &it->second;
	return nullptr;
}

void Server::sendCurrentModes(Client &client, Channel *channel)
{
    // Get the active modes for the channel using the getMode() function
    std::string modes = channel->getMode();

    if (modes.empty())
        modes = "No modes set";

    std::string response =
        ":ircserv(current mode) " + client.getNick() + " MODE " + channel->getChannelName() + " " + modes;
    sendToClient(client, response);
}

void Server::handleMode(Client &client, const std::string &channelName, const std::string &message)
{
    if (channelName.empty() || channelName[0] != '#')
    {
        std::cout << "ERROR: Invalid channel name" << std::endl;
        return;
    }

    Channel *channel = getChannelByChannelName(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
    }

    if (message.empty())
    {
        std::string currentModes = channel->getMode();
        std::cout << "DEBUG: Returning current mode: " << currentModes << std::endl;
        std::string response =
            ":ircserv(current mode) " + client.getNick() + " MODE " + channel->getChannelName() + " " + currentModes;

        sendToClient(client, response);
        return;
    }

    if (!channel->isOperator(&client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
        return;
    }

    // Check for valid mode changes and execute if valid
    if (checkForValidModes(message, client, channel))
        executeModes(client, channel);
}
