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

#include "../inc/Message.hpp"
#include "../inc/Server.hpp"
#include <functional>

void Server::handleClientMessage(Client &client, const std::string &message)
{
    std::cout << std::endl << "Received message: " << message;

    if (client.getState() == REGISTERING)
    {
        handleRegistration(client, message);
    }
    else
    {
        handleCommands(client, message);
    }
}

void Server::handleRegistration(Client &client, const std::string &message)
{
    std::cout << "Client is registering..." << std::endl;

    std::vector<std::string> tokens = split(message);
    if (tokens.size() < 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client.getNick(), "REGISTER"));
        return;
    }

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == "PASS" && i + 1 < tokens.size())
            Pass(client, tokens[i + 1]);
        else if (tokens[i] == "USER" && i + 4 < tokens.size())
            UserName(client, tokens[i + 1], tokens[i + 4]);
        else if (tokens[i] == "NICK" && i + 1 < tokens.size())
            Nick(client, tokens[i + 1]);
    }

    if (client.getUserNameOK() && client.getNickOK() && client.getPasswdOK())
    {
        client.setState(REGISTERED);
        sendToClient(client, RPL_WELCOME(client.getNick(), client.getUserName(), client.getHostName()));
        sendToClient(client, RPL_AVAILABLECMD(client.getNick()));
    }
}

void Server::handleCommands(Client &client, const std::string &message)
{
    std::string arg[3];
    std::istringstream iss(message);
    iss >> arg[0];

    std::map<std::string, std::function<void(Client &, std::istringstream &)>> registeredCommands = {
        {"PING",
         [](Client &c, std::istringstream &iss) {
             std::string arg1;
             iss >> arg1;
             if (arg1.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "PING"));
                 return;
             }
             sendToClient(c, "PONG " + arg1);
         }},
        {"NICK",
         [this](Client &c, std::istringstream &iss) {
             std::string arg1;
             iss >> arg1;
             if (arg1.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "NICK"));
                 return;
             }
             Nick(c, arg1);
         }},
        {"JOIN",
         [this](Client &c, std::istringstream &iss) {
             std::string channelName, password;
             iss >> channelName >> password;
             if (channelName.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "JOIN"));
                 return;
             }
             Join(c, channelName, password);
         }},
        {"TOPIC",
         [this](Client &c, std::istringstream &iss) {
             std::string channelName, newTopic;
             iss >> channelName;
             std::getline(iss, newTopic);
             if (channelName.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "TOPIC"));
                 return;
             }
             Topic(c, channelName, newTopic);
         }},
        {"PART",
         [this, &message](Client &c, std::istringstream &iss) {
             std::string arg1;
             iss >> arg1;
             if (arg1.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "PART"));
                 return;
             }
             Part(c, arg1, message);
         }},
        {"PRIVMSG",
         [this](Client &c, std::istringstream &iss) {
             std::string nickname, msg;
             iss >> nickname;
             std::getline(iss, msg);
             if (nickname.empty() || msg.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "PRIVMSG"));
                 return;
             }
             Privmsg(c, nickname, msg);
         }},
        {"INVITE",
         [this](Client &c, std::istringstream &iss) {
             std::string target, channelName;
             iss >> target;
             std::getline(iss, channelName);
             if (target.empty() || channelName.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "INVITE"));
                 return;
             }
             Invite(c, target, channelName);
         }},
        {"MODE",
         [this](Client &c, std::istringstream &iss) {
             std::string channelName, modeMessage;
             iss >> channelName;
             std::getline(iss, modeMessage);

             //  modeMessage = trim(modeMessage); // DO I NEED THIS?

             processModeCommand(c, channelName, modeMessage);
         }},
        {"NAMES",
         [this](Client &c, std::istringstream &iss) {
             std::string channelName;
             iss >> channelName;
             if (channelName.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "NAMES"));
                 return;
             }
             handleNamesCommand(c, channelName);
         }},
        {"KICK",
         [this](Client &c, std::istringstream &iss) {
             std::string channelName, target, reason;
             iss >> channelName >> target;
             std::getline(iss, reason);
             if (channelName.empty() || target.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "KICK"));
                 return;
             }
             Kick(c, channelName, target, reason);
         }},
        {"QUIT", [this, &message](Client &c, std::istringstream &iss) {
             (void)iss;
             if (message.empty())
             {
                 sendToClient(c, ERR_NEEDMOREPARAMS(c.getNick(), "QUIT"));
                 return;
             }
             Quit(c, message);
         }}};

    auto it = registeredCommands.find(arg[0]);
    if (it != registeredCommands.end())
    {
        it->second(client, iss);
    }
}
