#include "../../inc/Message.hpp"
#include "../../inc/Server.hpp"
#include <unordered_set>

void printOperatorList(Channel *channel) // DELETE DEGUG
{
    // Access the operator list via the public method
    const std::vector<Client *> &operatorList = channel->getOperatorList();

    if (operatorList.empty())
    {
        std::cout << "No operators set for channel " << channel->getChannelName() << std::endl;
        return;
    }

    std::cout << "Operator list for channel " << channel->getChannelName() << ":" << std::endl;

    // Range-based for loop to iterate over the operator list
    for (Client *operatorClient : operatorList)
    {
        std::cout << "- " << operatorClient->getNick() << std::endl;
    }
}

void Server::handleMode(char sign, char mode, Channel *channel, const std::vector<std::string> &parameters, int &i,
                        std::string &setModes, std::string &setParameters)
{
    switch (mode)
    {
    // USAGE: /MODE #channel_name +i / -i
    case 'i':
        if (sign == '+')
        {
            if (!channel->getInviteOnlyState())
            {
                channel->setInviteOnly();
                setModes += "+i";
            }
        }
        else if (sign == '-')
        {
            if (channel->getInviteOnlyState())
            {
                channel->unsetInviteOnly();
                setModes += "-i";
            }
        }
        break;
    case 'k':
        // USAGE: /MODE #channel_name +k password
        if (sign == '+')
        {
            channel->setChannelPassword(parameters[i]);
            setModes += "+k";
            if (setParameters.empty())
                setParameters += parameters[i];
            else
                setParameters += " " + parameters[i];
            i++;
        }
        else if (sign == '-')
        {
            if (!channel->getChannelPassword().empty())
            {
                channel->setChannelPassword("");
                setModes += "-k";
            }
        }
        break;

    case 'o':
        // USAGE: /MODE #channel_name +o user
        if (sign == '+')
        {
            Client *addOperator = getClientByNickname(parameters[i]);
            if (!channel->isOperator(addOperator))
            {
                channel->setOperator(addOperator);

                // Not sure if i need this
                sendToClient(*addOperator, RPL_YOUREOPER(addOperator->getNick(), channel->getChannelName()));
                
                if (setModes.find("+o") == std::string::npos)
                    setModes += "+o";
            }
            else
            {
                std::cout << parameters[i] << " is already an operator" << std::endl;
            }
            if (setParameters.empty())
                setParameters += parameters[i];
            else
                setParameters += " " + parameters[i];
            i++;
        }
        else if (sign == '-')
        {
            Client *removeOperator = getClientByNickname(parameters[i]);
            if (channel->isOperator(removeOperator))
            {
                channel->unsetOperator(removeOperator);
                setModes += "-o";
            }
            else
            {
                std::cout << parameters[i] << " is not an operator" << std::endl;
            }
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
    response = ":" + client.getNick() + "!~" + client.getUserName() + "@" + client.getHostName() + " MODE " +
               channel->getChannelName() + " " + setModes;

    if (!setParameters.empty())
        response += " " + setParameters;

    // i sen msg to everyone in the chat
    for (Client *member : channel->getUsers())
        sendToClient(*member, response);
}