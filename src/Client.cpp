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

Client::Client() {}

Client::Client(int fd, const sockaddr_in &client_addr)
    : _fd(fd), _addr(client_addr), _nick("*"), _userName(""), _passwdOK(false), _nickOK(false), _userNameOK(false) {}

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
