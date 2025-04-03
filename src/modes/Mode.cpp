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

void Server::processModeCommand(Client &client, const std::string &channelName, const std::string &modeMessage)
{
    // to prevent entering MODE functions for "Received message: MODE vkuznets +i"
    if (channelName[0] != '#')
        return;

    Channel *channel = getChannelByChannelName(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
    }

    if (!channel->isOperator(&client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
        return;
    }

    if (modeMessage.empty())
    {
        Server::sendCurrentModes(client, getChannelByChannelName(channelName));
        return;
    }

    // Check for valid mode changes and execute if valid
    if (checkForValidModes(modeMessage, client, channel))
        executeModes(client, channel);
    else
        sendToClient(client, ERR_UNKNOWNMODE(client.getNick(), modeMessage));
}
