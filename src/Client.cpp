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

#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include <arpa/inet.h>

Client::Client() {}

Client::Client(int fd, const sockaddr_in &client_addr)
	: _fd(fd), _addr(client_addr), _nick("*"), _userName(""), _passwdOK(false), _nickOK(false), _userNameOK(false)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getpeername(fd, (struct sockaddr*)&addr, &addr_len) == 0)
	{
		_host = inet_ntoa(addr.sin_addr);
	}
	else
	{
		_host = "unknown";
	}
}

Client::Client(const Client &other)
{
	this->_fd = other._fd;
	this->_state = other._state;
	this->_addr = other._addr;
	this->_nick = other._nick;
	this->_userName = other._userName;
	this->_passwdOK = other._passwdOK;
	this->_nickOK = other._nickOK;
	this->_userNameOK = other._userNameOK;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_fd = other._fd;
		this->_state = other._state;
		this->_addr = other._addr;
		this->_nick = other._nick;
		this->_userName = other._userName;
		this->_passwdOK = other._passwdOK;
		this->_nickOK = other._nickOK;
		this->_userNameOK = other._userNameOK;
	}
	return *this;
}

Client::~Client() {}

void Client::setFd(int value) { _fd = value; }
int Client::getFd() const { return (_fd); }

void Client::setState(clientState state) { _state = state; }
clientState Client::getState() const { return (static_cast<clientState>(_state)); }

void Client::setUserName(std::string userName) { _userName = userName; }
std::string Client::getUserName() const { return(_userName); }

void Client::setRealName(std::string realName) { _realName = realName; }
std::string Client::getRealName() const { return(_realName); }

void Client::setNick(std::string nick) { _nick = nick; }
std::string Client::getNick() const { return(_nick); }

void Client::setPasswdOK(bool ok) { _passwdOK = ok; }
bool Client::getPasswdOK() { return(_passwdOK); }

void Client::setNickOK(bool ok) { _nickOK = ok; }
bool Client::getNickOK() {return(_nickOK); }

void Client::setUserNameOK(bool ok) { _userNameOK = ok; }
bool Client::getUserNameOK() {return(_userNameOK); }


std::string Client::getHostName() const { return(_host); }

std::vector<Channel *> Client::getJoinedChannels() const { return _joinedChannels;}
void Client::setJoinedChannel(Channel *channel)
{
	auto it = std::find(_joinedChannels.begin(), _joinedChannels.end(), channel);
	if (it == _joinedChannels.end()) 
		_joinedChannels.push_back(channel);
}
void Client::unsetJoinedChannel(Channel *channel)
{
    auto it = std::find(_joinedChannels.begin(), _joinedChannels.end(), channel);
    
    if (it != _joinedChannels.end()) 
        _joinedChannels.erase(it);
}
