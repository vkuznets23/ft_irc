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

    bool _isInviteOnly;
    std::string _parsedModes;
    std::vector<std::string> _parsedParameters;

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

    // password
    void setChannelPassword(const std::string &password);
    void unsetChannelPassword();

    // user limit
    int getUserLimit();
    void setUserLimit(int limit);
    void unsetUserLimit();

    // topic
    void setTopic(Client *client, const std::string &topic);
    std::string getTopic() const;
    bool isTopicRestricted() const;

    // clients
    std::vector<Client *> getClients() const;

    // chanel modes
    std::string getMode() const;

    bool getInviteOnlyState();
    void setInviteOnly();
    void unsetInviteOnly();

    bool isOperator(Client *client) const;
    void setOperator(Client *client);


    void clearParsedParameters() { _parsedParameters.clear(); }
    std::vector<std::string> getParsedParameters() { return (_parsedParameters); }
    void setParsedParameters(std::vector<std::string> parameters) { _parsedParameters = parameters; }
    void setParsedModes(std::string modes) { _parsedModes = modes; }
    std::string getParsedModes() { return (_parsedModes); }
    std::vector<Client *> &getUsers() { return (_userList); }
};
