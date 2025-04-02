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

void Server::handleMode(Client &client, const std::string &channelName, const std::string &message)
{

    // check if operator send command (DONE)
    // parameters parsing
    // change channel states
    // send messages

    if (channelName[0] != '#')
        return;

    Channel *channel = getChannelByChannelName(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
    }
    // we should add logic to JOIN i guess?
    if (!channel->isOperator(&client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
        return;
    }

    // Check for valid mode changes and execute if valid
    if (checkForValidModes(message, client, channel))
        executeModes(client, channel);
}
