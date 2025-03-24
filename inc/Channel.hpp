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

#pragma once

#include "Client.hpp"
#include <vector>
#include <iostream>

class Client;

enum ChannelType
{
    PUBLIC,
    INVITE_ONLY,
    PRIVATE,
    MODERATED
};

class Channel
{
private:
    std::string _channelName;
    std::string _channelPassword;
    int _userLimit;
    std::string _topic;
    bool _topicOperatorsOnly;
    std::vector<Client *> _userList;
    std::vector<Client *> _operatorList;
    ChannelType _type;

public:
    // Constructors
    Channel();
    Channel(std::string name);
    Channel(const Channel &o);
    Channel &operator=(const Channel &o);
    ~Channel();

    // getters
    std::string getChannelName() const;
    std::string getChannelPassword() const;

    // client methods
    void addClient(Client *client);
    void removeClient(Client *client);
    bool isClientInChannel(Client *client);
    void setChannelPassword(const std::string &password);
    int getUserLimit();
    void setUserLimit(int limit);
    void setTopic(Client *client, const std::string &topic);
    std::string getTopic() const;

    // chanel modes
    void setChannelType(ChannelType type);
    ChannelType getChannelType() const;
};
