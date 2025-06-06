/* **************************************************************************************** */
/*                                                                                          */
/*                                                        ::::::::::: :::::::::   ::::::::  */
/*                                                           :+:     :+:    :+: :+:    :+:  */
/*                                                          :+:     :+:    :+: :+:          */
/*                                                         :+:     :+:+:+:+:  :+:           */
/*  By: Viktoriia Kuznetsova<vkuznets@student.hive.fi>    :+:     :+:    :+: :+:            */
/*      Juliette Mouette<jmouette@student.hive.fi>,      :+:     :+:    :+: :+:    :+:      */
/*      									                          ::::::::::: :::    :::  ::::::::        */
/*                                                                                          */
/* **************************************************************************************** */

#pragma once

#include "Client.hpp"

#include <algorithm>
#include <csignal>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <poll.h>

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 999;

class Client;
class Channel;

class Server
{
  private:
    int _port;
    std::string _password;
    std::vector<Client *> _clients;
	std::vector<struct pollfd> _pollFds;
    std::map<std::string, Channel> _channels;

  public:
    // constructors
    Server();
    Server(int port, std::string password);
    Server(const Server &o);
    Server &operator=(const Server &o);
    ~Server();

    void runServer();

    // run server
    int createServerSocket();
    void bindAndListen(int server_fd);
    void acceptNewConnection(int server_fd);

    // handleMessage for commands like
    static void sendToClient(Client client, const std::string &message);
    std::vector<std::string> split(const std::string &str);
    void handleRegistration(Client &client, const std::string &message);
    void handleCommands(Client &client, const std::string &message);
    void handleClientMessage(Client &client, const std::string &message);

    // commands
    void Pass(Client &client, const std::string &password);
    void UserName(Client &client, const std::string &username, const std::string &realname);
    bool isNickTaken(const std::string &nickname) const;
    bool isValidNick(const std::string &nickname) const;
    void Nick(Client &client, const std::string &nickname);
	void disablePollfd(int fd);
    void Quit(Client &client, std::string message);
    bool isChannelValid(Client &client, const std::string &channel);
    bool checkChannelType(Client &client, Channel &channel, const std::string &channelName,
                          const std::string &password);
    void Join(Client &client, std::string &channels, std::string &password);
    void Topic(Client &client, const std::string &channelName, const std::string &newTopic);
    void Part(Client &client, const std::string &channels, const std::string &message);
    bool checkBasicErrors(Client &client, const std::string &msgtarget, const std::string &message);
    std::vector<std::string> parseTargets(const std::string &msgtarget, Client &client);
    Client *findClient(const std::string &nick);
    void Privmsg(Client &client, const std::string &msgtarget, const std::string &message);
    void Invite(Client &client, const std::string &nickname, const std::string &channelName);
    void Kick(Client &client, const std::string &channelName, const std::string &target, std::string &reason);

    // clean up
    void cleanupResources(int server_fd);

    Channel *getChannelByChannelName(const std::string &channelName);
    bool checkForValidModes(const std::string &message, Client &client, Channel *channel);
    bool checkValidParameter(int index, std::vector<std::string> parameter, char mode, Channel *channel,
                             Client &client);
    Client *getClientByNickname(const std::string &nickname);
    bool userIsMemberOfChannel(Client &client, const std::string &channelName);
    void executeModes(Client &client, Channel *channel);
    std::string compressModes(const std::string &setModes);
    void handleNamesCommand(Client &client, const std::string &channelName);
    void processModeCommand(Client &client, const std::string &channelName, const std::string &modeMessage);
    void handlePlusModes(Channel *channel, char mode, const std::vector<std::string> &parameters, int &i,
                         std::string &setModes, std::string &setParameters); // NEW
    void handleMinusModes(Channel *channel, char mode, const std::vector<std::string> &parameters, int &i,
                          std::string &setModes, std::string &setParameters);
};

std::string trim(const std::string &str);