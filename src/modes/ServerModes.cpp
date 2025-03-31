#include "../../inc/Server.hpp"
#include "../../inc/Message.hpp"
#include <unordered_set>

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
    char currentSign;
    int i = 0;
    std::string setModes;
    std::string setParameters;

    std::vector<std::string> parameters = channel->getParsedParameters();

    for (char c : channel->getParsedModes())
    {
        if (c == '+' || c == '-')
            currentSign = c;
        else
        {
            if (currentSign == '+')
            {
                switch (c)
                {
                case 'i':
                    if (!channel->getInviteOnlyState())
                    {
                        channel->setInviteOnly();
                        setModes += "+i";
                    }
                    break;
                    // case 'k':
                    //     channel->setChannelPassword(parameters[i]);
                    //     setModes += "+k";
                    //     if (setParameters.empty())
                    //         setParameters += parameters[i];
                    //     else
                    //         setParameters += " " + parameters[i];
                    //     i++;
                    //     break;
                    // case 'l':
                    //     std::cout << "PARAMETER: " << parameters[i] << std::endl;
                    //     channel->setUserLimit(std::stoi(parameters[i]));
                    //     setModes += "+l";
                    //     if (setParameters.empty())
                    //         setParameters += parameters[i];
                    //     else
                    //         setParameters += " " + parameters[i];
                    //     i++;
                    //     break;
                }
            }
            else if (currentSign == '-')
            {
                switch (c)
                {
                case 'i':
                    if (channel->getInviteOnlyState())
                    {
                        channel->unsetInviteOnly();
                        setModes += "-i";
                    }
                    break;
                    // case 'k':
                    //     if (!channel->getChannelPassword().empty())
                    //     {
                    //         channel->setChannelPassword("");
                    //         setModes += "-k";
                    //     }
                    //     break;
                    // case 'l':
                    //     if (channel->getUserLimit() != -1)
                    //     {
                    //         channel->setUserLimit(-1);
                    //         setModes += "-l";
                    //     }
                    //     break;
                }
            }
        }
    }
    setModes = compressModes(setModes);
    response = ":" + client.getNick() + " " + "Mode" + " " + channel->getChannelName() + " " + setModes;
    if (!setParameters.empty())
        response += " " + setParameters;
    for (Client *member : channel->getUsers())
        sendToClient(*member, response);
}