#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Server.hpp"

void Server::handleNamesCommand(Client &client, const std::string &channelName)
{
    Channel *channel = getChannelByChannelName(channelName);

    if (!channel)
    {
        sendToClient(client, ":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }

    // Get user list in the channel
    std::string userList;
    for (Client *member : channel->getUsers())
    {
        if (channel->isOperator(member))
            userList += "@" + member->getNick() + " "; // '@' means the user is an operator
        else
            userList += member->getNick() + " ";
    }

    // Send responses
    sendToClient(client, ":ircserv 353 " + client.getNick() + " = " + channelName + " :" + userList);
    sendToClient(client, ":ircserv 366 " + client.getNick() + " " + channelName + " :End of /NAMES list");
}
