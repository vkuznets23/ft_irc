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

const std::string SERVERNAME = "ircserv";

//generic
#define ERR_NEEDMOREPARAMS(nickname, command)						":" + SERVERNAME + " 461 " + nickname + " " + command + " :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(nickname, command)						":" + SERVERNAME + " 421 " + nickname + " " + command + " :Unknown command"

//authentification
#define RPL_WELCOME(nickname, username, host)						":" + SERVERNAME + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!~" + username + "@" + host

//username
#define ERR_ALREADYREGISTRED()										":" + SERVERNAME + " 462 :Unauthorized command (already registered)"

//nickname
#define RPL_NICKCHANGE(oldnick, username, host, nickname)			":" + oldnick + "!" + username + "@" + host + " NICK :" + nickname
#define ERR_NOSUCHNICK(nickname, nicktofind)						":" + SERVERNAME + " 401 " + nickname + " " + nicktofind + " :No such nick/channel"
#define ERR_NONICKNAMEGIVEN(nickname)								":" + SERVERNAME + " 431 " + nickname + " :No nickname given"
#define ERR_NICKNAMEINUSE(oldnick, nickname)						":" + SERVERNAME + " 433 " + oldnick + " " + nickname  + " :Nickname is already in use"

//password
#define RPL_PASSWDOK()												":" + SERVERNAME + " NOTICE Password is correct!"
#define ERR_PASSWDMISMATCH(nickname)								":" + SERVERNAME + " 464 " + nickname + " :Password is incorrect"

//quit
#define RPL_QUIT(nickname, username, hostname, message)				":" + nickname + "!~" + username + "@" + hostname + " QUIT :" + message

//topic
#define RPL_NOTOPIC(channel)										":" + SERVERNAME + " NOTICE " + channel + " :No topic is set"
#define RPL_TOPIC(channel, topic)									":" + SERVERNAME + " NOTICE " + channel + " :" + topic 
#define RPL_NEWTOPIC(channel, topic)								":" + SERVERNAME + " NOTICE Topic for " + channel + " changed to :" + topic 

//channel
#define ERR_NOSUCHCHANNEL(nickname, channel)						":" + SERVERNAME + " 403 " + nickname + " " + channel + " :No such channel"
#define ERR_USERNOTINCHANNEL(nickname, channel)						":" + SERVERNAME + " 441 " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(nickname, channel)						":" + SERVERNAME + " 441 " + nickname + " " + channel + " :You're not channel operator"
#define ERR_CHANNELISFULL(nickname, channel)						":" + SERVERNAME + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN(nickname, channel)						":" + SERVERNAME + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN(nickname, channel)						":" + SERVERNAME + " 474 " + nickname + " " + channel + " :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(nickname, channel)						":" + SERVERNAME + " 475 " + nickname + " " + channel + " :Cannot join channel (+k)"
#define RPL_NAMREPLY(nickname, channel)								":" + SERVERNAME + " 353 " + nickname + " @ " + channel + " :@" + nickname
#define RPL_ENDOFNAMES(nickname, channel)							":" + SERVERNAME + " 366 " + nickname + " " + channel + " :End of /NAMES list"
#define RPL_YOUREOPER(nickname, channel)							":" + SERVERNAME + " NOTICE " + channel + ":You are now an IRC operator"

//join
#define RPL_JOIN(nickname, username, hostname, channel)				":" + nickname + "!~" + username + "@" + hostname + " JOIN " + channel
#define RPL_JOINWELCOME(nickname, channel)							":" + SERVERNAME + " NOTICE " + nickname + " :Welcome to the channel :" + channel + "!"

//part
#define RPL_PART(nickname, username, hostname, channel, message)	":" + nickname + "!~" + username + "@" + hostname + " PART " + channel + " :" + message


#define RPL_AVAILABLECMD(nickname) \
    ":" + SERVERNAME + " NOTICE " + nickname + " :Available commands:" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  NICK     - Change your nickname" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  JOIN     - Join a channel" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  PART     - Leave a channel" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  PRIVMSG  - Send a message to a user or channel" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  MODE     - Set user or channel modes" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  TOPIC    - View or change channel topic" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  QUIT     - Disconnect from the server" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  INVITE   - Invite a user to a channel" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  KICK     - Remove a user from a channel" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :  HELP     - Show this help message" + "\n" \
    ":" + SERVERNAME + " NOTICE " + nickname + " :Type HELP <command> for more information about a specific command"
