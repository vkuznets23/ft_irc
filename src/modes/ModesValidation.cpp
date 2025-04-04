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
#include <unordered_set>

bool Server::checkValidParameter(int index, std::vector<std::string> parameter, char mode, Channel *channel,
                                 Client &client)
{
    std::string resp;

    if (mode == 'l' && !std::all_of(parameter[index].begin(), parameter[index].end(), ::isdigit))
        return (false);
    if (mode == 'o')
    {
        Client *potentialMember = getClientByNickname(parameter[index]);
        if (potentialMember == nullptr)
        {
            sendToClient(client, ERR_USERNOTINCHANNEL(parameter[index], channel->getChannelName()));
            return (false);
        }
        else if (!userIsMemberOfChannel(client, channel->getChannelName()))
        {
            sendToClient(client, ERR_USERNOTINCHANNEL(parameter[index], channel->getChannelName()));
            return (false);
        }
        return (true);
    }
    return true;
}

bool Server::checkForValidModes(const std::string &message, Client &client, Channel *channel)
{
    std::string modes;
    char currentSign = '\0';
    int neededParameterCount = 0;
    int index = 0;
    std::unordered_set<char> allowedChars = {'+', '-', 'i', 'k', 'l', 'o', 't'};

    std::istringstream iss(message);
    iss >> modes;
    for (char c : modes)
    {
        // check for allowed chars
        if (allowedChars.find(c) == allowedChars.end())
        {
            sendToClient(client, ERR_UNKNOWNMODE(client.getNick(), c));
            return false;
        }

        // check if we add or remove
        if (c == '+' || c == '-')
            currentSign = c;
        if (currentSign == '+')
        {
            if (c == 'k' || c == 'l' || c == 'o')
                neededParameterCount++;
        }
        else if (currentSign == '-')
        {
            if (c == 'o')
            {
                neededParameterCount++;
            }
        }
    }
    if (neededParameterCount > 0)
    {
        std::vector<std::string> parameter(neededParameterCount);
        for (int i = 0; i < neededParameterCount; i++)
            iss >> parameter[i];
        for (char c : modes)
        {
            if (c == '+' || c == '-')
            {
                currentSign = c;
            }
            else
            {
                if (currentSign == '+')
                {
                    if (c == 'k' || c == 'l' || c == 'o')
                    {
                        if (!checkValidParameter(index, parameter, c, channel, client))
                            return false;
                        index++;
                    }
                }
                if (currentSign == '-')
                {
                    if (c == 'o')
                    {
                        if (!checkValidParameter(index, parameter, c, channel, client))
                            return false;
                        index++;
                    }
                }
            }
        }
        channel->clearParsedParameters();
        channel->setParsedParameters(parameter);
        channel->setParsedModes(modes);
        return (true);
    }
    channel->clearParsedParameters();
    channel->setParsedModes(modes);
    return (true);
}