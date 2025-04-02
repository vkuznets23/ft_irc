#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"
#include <unordered_set>

void Server::handleMode(char sign, char mode, Channel *channel, const std::vector<std::string> &parameters, int &i,
                        std::string &setModes, std::string &setParameters)
{
    if (!channel->getInviteOnlyState())
        std::cout << "bitch" << std::endl;
    switch (mode)
    {
    // USAGE: /MODE #channel_name +i / -i
    case 'i':
        if (sign == '+')
        {
            if (!channel->getInviteOnlyState())
            {
                std::cout << "+i mode" << std::endl;
                channel->setInviteOnly();
                setModes += "+i";
            }
        }
        else if (sign == '-')
        {
            if (channel->getInviteOnlyState())
            {
                std::cout << "-i mode" << std::endl;
                channel->unsetInviteOnly();
                setModes += "-i";
            }
        }
        break;

    case 'o':
        if (sign == '+')
        {
            Client *addOperator = getClientByNickname(parameters[i]);
            channel->setOperator(addOperator);
            std::cout << parameters[i] << " set as operator" << std::endl;
            setModes += "+o";
            if (setParameters.empty())
                setParameters += parameters[i];
            else
                setParameters += " " + parameters[i];
            i++;
        }
        break;

        // Add other modes (e.g., 'k', 'l') here with similar logic if required
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
    std::vector<std::string> parameters = channel->getParsedParameters();
    std::string modes = channel->getParsedModes();

    for (size_t j = 0; j < modes.size(); ++j)
    {
        char sign = modes[j];
        if (sign == '+' || sign == '-')
        {
            // Ensure there's a mode character to process
            if (j + 1 < modes.size())
            {
                handleMode(sign, modes[j + 1], channel, parameters, i, setModes, setParameters);
                j++; // Skip next character as it's the mode character (e.g., 'o', 'i', etc.)
            }
        }
    }

    setModes = compressModes(setModes);
    response = ":ircserv(execut) " + client.getNick() + " " + "MODE" + " " + channel->getChannelName() + " " + setModes;

    if (!setParameters.empty())
        response += " " + setParameters;

    for (Client *member : channel->getUsers())
        sendToClient(*member, response);
}