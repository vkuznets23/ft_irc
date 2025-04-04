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

#include "../inc/Server.hpp"

/******************************** CONSTRUCTORS ********************************/

Server::Server() {};

Server::Server(int port, std::string password) : _port(port), _password(password) {};

Server::Server(const Server &o)
{
    this->_port = o._port;
    this->_password = o._password;
}

Server &Server::operator=(const Server &o)
{
    if (this != &o)
    {
        this->_port = o._port;
        this->_password = o._password;
    }
    return *this;
}

Server::~Server() {};

/******************************** GETTERS NICKNAME/CHANNELNAME ********************************/

Client *Server::getClientByNickname(const std::string &nickname)
{
    auto it = std::find_if(_clients.begin(), _clients.end(),
                           [&nickname](Client *client) { return client->getNick() == nickname; });

    return (it != _clients.end()) ? *it : nullptr;
}

Channel *Server::getChannelByChannelName(const std::string &channelName)
{
    auto it = _channels.find(channelName);

    if (it != _channels.end())
        return &it->second;
    return nullptr;
}

/******************************** REST ********************************/

bool Server::userIsMemberOfChannel(Client &client, const std::string &channelName)
{
    Channel *channel = getChannelByChannelName(channelName);
    if (!channel)
        return false;

    return channel->isClientInChannel(&client);
}
