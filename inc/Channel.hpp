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
#include "Server.hpp"
#include <ctime>
#include <iostream>
#include <set>
#include <vector>

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
    std::string _timestamp;
    Client *_operatorClient = nullptr;
    std::set<std::string> _invitedUsers;
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

    // Getters & Setters
    std::string getChannelName() const;
    void setTimestamp();
    std::string getTimestamp() const;

    // Client Methods
    void addClient(Client &client);
    void removeClient(Client *client);
    bool isClientInChannel(Client *client);
    std::vector<Client *> &getUsers();

    // Operator Methods
    void setOperator(Client *client);
    void unsetOperator(Client *client);
    Client *getOperator() const;
    bool isOperator(Client *client) const;

    // Display message to all clients in the channel
    void displayChannelMessagePrivMsg(Client &sender, const std::string &message);
    void displayChannelMessageKick(Client &sender, const std::string &message, const std::string &target);
    void displayChannelMessageTopic(Client &sender, const std::string &message);

    // Invite
    void addInvite(const std::string &nickname);
    bool isInvited(const std::string &nickname) const;
    void removeInvite(const std::string &nickname);

    // password
    std::string getChannelPassword() const;
    void setChannelPassword(const std::string &password);
    void unsetChannelPassword();

    // user limit
    int getUserLimit() const;
    void setUserLimit(int limit);
    void unsetUserLimit();
    int getUserCount() const;

    // topic
    void setTopic(const std::string &topic);
    std::string getTopic() const;
    bool isTopicRestricted() const;

    // chanel modes
    std::string getMode() const;

    // inviteOnly
    bool getInviteOnlyState();
    void setInviteOnly();
    void unsetInviteOnly();

    void clearParsedParameters()
    {
        _parsedParameters.clear();
    }
    std::vector<std::string> getParsedParameters()
    {
        return (_parsedParameters);
    }
    void setParsedParameters(std::vector<std::string> parameters)
    {
        _parsedParameters = parameters;
    }
    void setParsedModes(std::string modes)
    {
        _parsedModes = modes;
    }
    std::string getParsedModes()
    {
        return (_parsedModes);
    }

    // DELETE/DEBUG
    std::vector<Client *> getOperatorList() const
    {
        return _operatorList; // Returns the list of operators
    }
};
