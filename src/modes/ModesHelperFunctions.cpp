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

void Server::handlePlusModes(Channel *channel, char mode, const std::vector<std::string> &parameters, int &i,
                             std::string &setModes, std::string &setParameters)
{
    switch (mode)
    {
    case 'i':
        if (!channel->getInviteOnlyState())
        {
            channel->setInviteOnly();
            setModes += "+i";
        }
        break;
    case 'k':
        channel->setChannelPassword(parameters[i]);
        setModes += "+k";
        if (setParameters.empty())
            setParameters += parameters[i];
        else
            setParameters += " " + parameters[i];
        i++;
        break;
    case 'l':
        channel->setUserLimit(std::stoi(parameters[i]));
        setModes += "+l";
        if (setParameters.empty())
            setParameters += parameters[i];
        else
            setParameters += " " + parameters[i];
        i++;
        break;
    case 't':
        if (!channel->isTopicRestricted())
        {
            channel->setTopicRestricted(true);
            setModes += "+t";
        }
        break;
    case 'o':
        Client *addOperator = getClientByNickname(parameters[i]);
        if (!channel->isOperator(addOperator))
        {
            channel->setOperator(addOperator);
            setModes += "+o";
            if (setParameters.empty())
                setParameters += parameters[i];
            else
                setParameters += " " + parameters[i];
        }
        i++;
        break;
    }
}

void Server::handleMinusModes(Channel *channel, char mode, const std::vector<std::string> &parameters, int &i,
                              std::string &setModes, std::string &setParameters)
{
    switch (mode)
    {
    case 'i':
        if (channel->getInviteOnlyState())
        {
            channel->unsetInviteOnly();
            setModes += "-i";
        }
        break;
    case 'k':
        channel->unsetChannelPassword();
        setModes += "-k";
        break;
    case 'l':
        if (channel->getUserLimit() != -1)
        {
            channel->setUserLimit(-1);
            setModes += "-l";
        }
        break;
    case 't':
        if (channel->isTopicRestricted())
        {
            channel->setTopicRestricted(false);
            setModes += "-t";
        }
        break;
    case 'o':
        Client *removeOperator = getClientByNickname(parameters[i]);
        if (channel->isOperator(removeOperator))
        {
            channel->unsetOperator(removeOperator);

            setModes += "-o";
            if (setParameters.empty())
                setParameters += parameters[i];
            else
                setParameters += " " + parameters[i];
        }
        i++;
        break;
    }
}

std::string Server::compressModes(const std::string &setModes)
{
    std::string result;
    std::string currentGroup;
    char currentSign = '\0';

    for (char c : setModes)
    {
        if (c == '+' || c == '-')
        {
            if (!currentGroup.empty())
            {
                result += currentSign + currentGroup;
                currentGroup.clear();
            }
            currentSign = c;
        }
        else
            currentGroup += c;
    }
    if (!currentGroup.empty())
        result += currentSign + currentGroup;
    return (result);
}