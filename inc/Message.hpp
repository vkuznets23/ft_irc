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

#include "Server.hpp"
#include <iostream>

// generic
#define ERR_NEEDMOREPARAMS(nickname, command) ":" + SERVERNAME + " 461 " + nickname + " " + command + " :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(nickname, command) ":" + SERVERNAME + " 421 " + nickname + " " + command + " :Unknown command"

// authentification
#define RPL_WELCOME(nickname, username, host) ":" + SERVERNAME + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!~" + username + "@" + host

// username
#define ERR_ALREADYREGISTRED() ":" + SERVERNAME + " 462 :Unauthorized command (already registered)"

// nickname
#define RPL_NICKCHANGE(oldnick, username, host, nickname) ":" + oldnick + "!" + username + "@" + host + " NICK :" + nickname
#define ERR_NOSUCHNICK(nickname, nicktofind) ":" + SERVERNAME + " 401 " + nickname + " " + nicktofind + " :No such nick/channel"
#define ERR_NONICKNAMEGIVEN(nickname) ":" + SERVERNAME + " 431 " + nickname + " :No nickname given"
#define ERR_NICKNAMEINUSE(oldnick, nickname) ":" + SERVERNAME + " 433 " + oldnick + " " + nickname + " :Nickname is already in use"

// password
#define RPL_PASSWDOK() "NOTICE * Password is correct!"
#define ERR_PASSWDMISMATCH(nickname) ":" + SERVERNAME + " 464 " + nickname + " :Password is incorrect"

// topic
#define RPL_NOTOPIC(channel) "NOTICE * " + channel + " :No topic is set"
#define RPL_TOPIC(channel, topic) "NOTICE * " + channel + " :" + topic
#define RPL_NEWTOPIC(channel, topic) "NOTICE * Topic for " + channel + " changed to :" + topic

// channel
#define ERR_NOSUCHCHANNEL(nickname, channel) ":" + SERVERNAME + " 403 " + nickname + " " + channel + " :No such channel"
#define ERR_USERNOTINCHANNEL(nickname, channel) ":" + SERVERNAME + " 441 " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) ":" + SERVERNAME + " 441 " + nickname + " " + channel + " :You're not channel operator"
#define ERR_CHANNELISFULL(nickname, channel) ":" + SERVERNAME + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(nickname, channel) ":" + SERVERNAME + " 475 " + nickname + " " + channel + " :Cannot join channel (+k)"
#define ERR_UNKNOWNMODE(nickname, mode) ":" + SERVERNAME + " 472 " + nickname + " " + mode + " :Is unknown mode char to me"

// join
#define RPL_JOIN(nickname, channel) "NOTICE * " + nickname + " join :" + channel