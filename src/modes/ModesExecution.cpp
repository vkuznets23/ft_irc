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

#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"

void Server::executeModes(Client &client, Channel *channel)
{
    std::string response;
    std::string setModes;
    std::string setParameters;
    int i = 0;
    char currentSign;

    std::vector<std::string> parameters = channel->getParsedParameters();
    std::string modes = channel->getParsedModes();

    for (char c : modes)
    {
        if (c == '+' || c == '-')
            currentSign = c;
        else
        {
            if (currentSign == '+')
                handlePlusModes(channel, c, parameters, i, setModes, setParameters);
            else if (currentSign == '-')
                handleMinusModes(channel, c, parameters, i, setModes, setParameters);
        }
    }

    setModes = compressModes(setModes);
    response = ":" + client.getNick() + "!~" + client.getUserName() + "@" + client.getHostName() + " MODE " +
               channel->getChannelName() + " " + setModes;

    if (!setParameters.empty())
        response += " " + setParameters;

    for (Client *member : channel->getUsers())
        sendToClient(*member, response);
}

void Server::processModeCommand(Client &client, const std::string &channelName, const std::string &modeMessage)
{
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

    // Check for valid mode changes and execute if valid
    if (checkForValidModes(modeMessage, client, channel))
        executeModes(client, channel);
    else
        sendToClient(client, ERR_UNKNOWNMODE(client.getNick(), modeMessage));
}