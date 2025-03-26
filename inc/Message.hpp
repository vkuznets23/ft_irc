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

//generic
#define ERR_NEEDMOREPARAMS(nickname, command)                       ":" + SERVERNAME + " 461 " + nickname + " " + command + " :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(nickname, command)                       ":" + SERVERNAME + " 421 " + nickname + " " + command + " :Unknown command"

//authentification
#define RPL_WELCOME(nickname, username, host)                       ":" + SERVERNAME + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!~" + username + "@" + host

//nickname
#define RPL_NICKCHANGE(oldnick, username, host, nickname)			":" + oldnick + "!" + username + "@" + host + " NICK :" + nickname
#define ERR_NOSUCHNICK(nickname, nicktofind)                        ":" + SERVERNAME + "401 " + nickname + " " + nicktofind + " :No such nick/channel"
#define ERR_NONICKNAMEGIVEN(nickname)                        		":" + SERVERNAME + "431 " + nickname + " :No nickname given"
#define ERR_NICKNAMEINUSE(oldnick, nickname)                        ":" + SERVERNAME + "433 " + oldnick + " " + nickname  + " :Nickname is already in use"

// //channel
// #define RPL_CREATIONTIME(nickname, channelName, timestamp)          ":" + SERVERNAME + "329 " + nickname + " " + channelName + " " + timestamp
// #define ERR_NOSUCHCHANNEL(nickname, channelname)                    ":" + SERVERNAME + "403 " + nickname + " " + channelname + " :No such channel"
// #define ERR_USERNOTINCHANNEL(clientnick, usernickname, channelname) ":" + SERVERNAME + "441 " + nick + " " + usernickname + " " + channelname + " :They aren't on that channel"
// #define ERR_NOTONCHANNEL(clientnick, channelname)                   ":" + SERVERNAME + "442 " + clientnick + " " + channelname + " :They aren't on that channel"
// #define ERR_USERONCHANNEL(clientnick, usernick, channelname)        ":" + SERVERNAME + "443 " + clientnick + " " + nick + " " + channelName + " :is already on channel"
// #define ERR_CHANOPRIVSNEEDED(nickname, channelname)                 ":" + SERVERNAME + "482 " + nickname + " " + channelname + " :You're not channel operator"
// #define ERR_NONICKNAMEGIVEN()                                       ":" + SERVERNAME + "431 :Nickname not given"
// #define ERR_PASSWDMISMATCH(source)                                  ":" + SERVERNAME + "464 " + source + " :Password is incorrect"

// //modes
// #define RPL_CHANNELMODEIS(nickname, channelName, channelModes)      ":" + SERVERNAME + "324 " + nickname + " " + channelName + " " + channelModes
// #define ERR_CHANNELISFULL(nickname, channelName)                    ":" + SERVERNAME + "471 " + nickname + " " + channelName + " :Cannot join channel (+l) - channel is full, try again later"
// #define ERR_UNKNOWNMODE(nickname, mode)                             ":" + SERVERNAME + "472 " + nickname + " " + mode + " :is an unknown mode char to me"
// #define ERR_INVITEONLYCHAN(nickname, channelName)                   ":" + SERVERNAME + "473 " + nickname + " " + channelName + " :Cannot join channel (+i) - you must be invited"
// #define ERR_BADCHANNELKEY(nickname, channelName)                    ":" + SERVERNAME + "475 " + nickname + " " + channelName + " :Cannot join channel (+k) - bad key"

// /* Numeric Responses */
// #define RPL_PASSWDOK()                                              "NOTICE * Password is correct!"
// #define RPL_PASSWDREQUEST()                                         "NOTICE * :This server requires a password. Please send: PASS <password>"
// #define RPL_NICKREQUEST()                                           "NOTICE * :This server requires a user nickname. Please send: NICK <nickname>"
// #define RPL_USERNAMEREQUEST()                                       "NOTICE * :This server requires a user user name. Please send: User <username username localhost :Name>"
// #define RPL_NAMREPLY(nickname, channelname, users)                  ":" + SERVERNAME + "353 " + nickname + " @ " + channelname + " :@" + users
// #define RPL_ENDOFNAMES(source, channelname)                         ":" + SERVERNAME + "366 " + source + " " + channelname + " :End of /NAMES list."

// /* Command Responses */
// #define RPL_INVITING(clientnick, nickname, channelname)		        ":" + SERVERNAME + "341 " + clientnick + " " + nickname + " " + channelname
// #define RPL_NICK(oldnick, username, nick)				            ":" + oldNick + " NICK :" + nick
// #define RPL_TOPIC(clientnick, channelname, newtopic)	            ":" + clientnick + " TOPIC " + channelName + " " + newtopic
// #define RPL_JOIN(source, channel)                                   ":" + source + " JOIN :" + channel
// #define RPL_KICK(source, channel, target, reason)                   ":" + source + " KICK " + channel + " " + target + " :" + reason
// #define RPL_PRIVMSG(clientnick, nick, message)                      ":" + clientnick + " PRIVMSG " + nick + " :" + message
