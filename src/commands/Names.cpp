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

void Server::handleNamesCommand(Client &client, const std::string &channelName)
{
    Channel *channel = getChannelByChannelName(channelName);

    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
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
    sendToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
}
