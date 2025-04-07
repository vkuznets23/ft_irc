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

#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"

/**
 * This function sends the list of nicknames of users currently in the specified channel
 * to the client. It follows these steps:
 * 
 * 1. Checks whether the channel exists. If not, sends an error message to the client.
 * 2. Iterates over all users in the channel.
 *    - Adds an "@" prefix to nicknames of channel operators to indicate their status.
 * 3. Sends a formatted list of users to the client using numeric reply 353.
 * 4. Sends the end of names list notification (numeric reply 366).
 */

void Server::handleNamesCommand(Client &client, const std::string &channelName)
{
    Channel *channel = getChannelByChannelName(channelName);

    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
    }

    std::string userList;
    for (Client *member : channel->getUsers())
    {
        if (channel->isOperator(member))
            userList += "@" + member->getNick() + " ";
        else
            userList += member->getNick() + " ";
    }

    sendToClient(client, ":ircserv 353 " + client.getNick() + " = " + channelName + " :" + userList);
    sendToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
}
