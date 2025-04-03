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

#include "../inc/Channel.hpp"

/******************************** CONSTRUCTORS ********************************/
Channel::Channel() {};
Channel::Channel(std::string name)
    : _channelName(name), _channelPassword(""), _userLimit(-1), _topicOperatorsOnly(false), _isInviteOnly(false) {};

Channel::Channel(const Channel &o)
{
    this->_channelName = o._channelName;
    this->_channelPassword = o._channelPassword;
    this->_userLimit = o._userLimit;
}

Channel &Channel::operator=(const Channel &o)
{
    if (this != &o)
    {
        this->_channelName = o._channelName;
        this->_channelPassword = o._channelPassword;
        this->_userLimit = o._userLimit;
    }
    return *this;
}

Channel::~Channel() {};

/******************************** GENERAL CHANNEL GETTERS AND SETTERS ********************************/
std::string Channel::getChannelName() const
{
    return (_channelName);
}

std::string Channel::getChannelPassword() const
{
    return _channelPassword.empty() ? "" : _channelPassword;
}
void Channel::setChannelPassword(const std::string &password)
{
    _channelPassword = password;
}
void Channel::unsetChannelPassword()
{
    _channelPassword.clear();
}

/******************************** CLIENT METHODS ********************************/
void Channel::addClient(Client &client)
{
    _userList.push_back(&client);
}

void Channel::removeClient(Client *client)
{
    auto it = std::find(_userList.begin(), _userList.end(), client);
    if (it != _userList.end())
    {
        _userList.erase(it);
    }
}

bool Channel::isClientInChannel(Client *client)
{
    for (Client *user : _userList)
    {
        if (user == client)
            return true;
    }
    return false;
}

int Channel::getUserLimit() const
{
    return (_userLimit);
}
void Channel::setUserLimit(int limit)
{
    _userLimit = limit;
}
void Channel::unsetUserLimit()
{
    _userLimit = -1;
}

int Channel::getUserCount() const
{
    return _userList.size();
}

std::vector<Client *> &Channel::getUsers() { return (_userList); }

/******************************** TOPIC METHODS (OPERATOR) ********************************/
std::string Channel::getTopic() const
{
    return _topic;
}
void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}
bool Channel::isTopicRestricted() const
{
    return _topicOperatorsOnly;
}

/******************************** OPERATORS ********************************/
void Channel::setOperator(Client *client)
{
    auto it = std::find(_operatorList.begin(), _operatorList.end(), client);
    if (it == _operatorList.end())
        _operatorList.push_back(client);
}

void Channel::unsetOperator(Client *client)
{
    auto it = std::find(_operatorList.begin(), _operatorList.end(), client);
    if (it == _operatorList.end())
        _operatorList.erase(it);
}

Client *Channel::getOperator() const
{
    return _operatorClient;
}
bool Channel::isOperator(Client *client) const
{
    return std::find(_operatorList.begin(), _operatorList.end(), client) != _operatorList.end();
}

/******************************** TIMESTAMP ********************************/
void Channel::setTimestamp()
{
    std::time_t now = std::time(nullptr);
    _timestamp = std::asctime(std::localtime(&now));
}

std::string Channel::getTimestamp() const
{
    return _timestamp;
}

/******************************** DISPLAY MESSAGE ********************************/
void Channel::displayChannelMessagePrivMsg(Client &sender, const std::string &message)
{
    std::string cleanedMessage = message;
    cleanedMessage.erase(
        std::find_if(cleanedMessage.rbegin(), cleanedMessage.rend(), [](unsigned char ch) { return !std::isspace(ch); })
            .base(),
        cleanedMessage.end());

    std::string fullMsg = ":" + sender.getNick() + " PRIVMSG " + _channelName + " :" + cleanedMessage;

    for (Client *client : _userList)
    {
        if (client != &sender)
        {
            Server::sendToClient(*client, fullMsg);
        }
    }
}

void Channel::displayChannelMessageKick(Client &sender, const std::string &message, const std::string &target)
{
    std::string cleanedMessage = message;
    cleanedMessage.erase(
        std::find_if(cleanedMessage.rbegin(), cleanedMessage.rend(), [](unsigned char ch) { return !std::isspace(ch); })
            .base(),
        cleanedMessage.end());

    std::string fullMsg = ":" + sender.getNick() + " KICK " + _channelName + " " + target + " " + cleanedMessage;

    for (Client *client : _userList)
    {
        if (client != &sender)
        {
            Server::sendToClient(*client, fullMsg);
        }
    }
}

void Channel::displayChannelMessageTopic(Client &sender, const std::string &message)
{
	std::string cleanedMessage = message;

	cleanedMessage.erase(std::find_if(cleanedMessage.rbegin(), cleanedMessage.rend(), 
					[](unsigned char ch) { return !std::isspace(ch); }).base(), cleanedMessage.end());

	std::string fullMsg = ":" + sender.getNick() + " TOPIC " + _channelName + " " + cleanedMessage;

	for (Client *client : _userList)
	{
		if (client != &sender)
		{
			Server::sendToClient(*client, fullMsg);
		}
	}
}

/******************************** INVITE ********************************/
void Channel::addInvite(const std::string &nickname)
{
    _invitedUsers.insert(nickname);
}

bool Channel::isInvited(const std::string &nickname) const
{
    return _invitedUsers.find(nickname) != _invitedUsers.end();
}

void Channel::removeInvite(const std::string &nickname)
{
    _invitedUsers.erase(nickname);
}

// i - invite only
// k - Password Protected
// l - user limit
// t - Topic Operators Only
// o - Give/take channel operator privilege

// USAGE: [/mode #channel +i ] [/mode #channel -i]
// USAGE: [/mode #channel +k (mypassword)] [/mode #channel -k]
// USAGE: [/mode #channel +l (limit)] [/mode #channel -l]
// USAGE: [/mode #channel +t ] [/mode #channel -t]
// OR [/mode #channel +i +k mypassword +l 50]
/******************************** INVITE ONLY ********************************/
std::string Channel::getMode() const
{
    std::string activeModes;

    if (_isInviteOnly)
        activeModes += " +i";

    if (!_channelPassword.empty())
        activeModes += " +k " + _channelPassword;

    if (_userLimit != -1)
        activeModes += " +l " + std::to_string(_userLimit);

    if (_topicOperatorsOnly)
        activeModes += " +t";

    if (!activeModes.empty() && activeModes[0] == ' ')
        activeModes = activeModes.substr(1);

    return activeModes;
}

bool Channel::getInviteOnlyState()
{
    return (_isInviteOnly);
}
void Channel::setInviteOnly()
{
    _isInviteOnly = true;
}
void Channel::unsetInviteOnly()
{
    _isInviteOnly = false;
}
