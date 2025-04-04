#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"
#include <unordered_set>

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
        if (setParameters.empty())
            setParameters += parameters[i];
        else
            setParameters += " " + parameters[i];
        i++;
        break;
    case 'l':
        if (channel->getUserLimit() != -1)
        {
            channel->setUserLimit(-1);
            setModes += "-l";
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

    // i sen msg to everyone in the chat
    for (Client *member : channel->getUsers())
        sendToClient(*member, response);
}