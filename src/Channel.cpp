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
Channel::Channel(std::string name) : _channelName(name), _channelPassword(""), _userLimit(-1), _topicOperatorsOnly(false) {};

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

/******************************** GETTERS ********************************/
std::string Channel::getChannelName() const { return (_channelName); }

std::string Channel::getChannelPassword() const { return _channelPassword.empty() ? "No password set" : _channelPassword; }
void Channel::setChannelPassword(const std::string &password) { _channelPassword = password; }

/******************************** CLIENT METHODS ********************************/
void Channel::addClient(Client *client) { _userList.push_back(client); }

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

int Channel::getUserLimit() { return (_userLimit); }
void Channel::setUserLimit(int limit) { _userLimit = limit; }

std::vector<Client*> Channel::getClients() const { return _userList; }
bool Channel::isTopicRestricted() const { return _topicOperatorsOnly;}

/******************************** OPERATORS ONLY METHODS ********************************/
std::string Channel::getTopic() const { return _topic; }

/*user with operator rights only*/
void Channel::setTopic(Client *client, const std::string &topic)
{
	if (!isClientInChannel(client))
		throw std::runtime_error("Client is not in the channel!");
	if (_topicOperatorsOnly)
		throw std::runtime_error("Client is not operator!");
	else
		_topic = topic;
}

/******************************** CHANNEL TYPES ********************************/
void Channel::setChannelType(ChannelType type) { _type = type; }
ChannelType Channel::getChannelType() const { return _type; }
