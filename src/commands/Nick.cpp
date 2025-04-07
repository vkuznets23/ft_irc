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

#include "../../inc/Server.hpp"
#include "../../inc/Message.hpp"

bool Server::isValidNick(const std::string &nickname) const
{
	if (isdigit(nickname[0]))
        return (false);

	for (char c : nickname)
	{
		if (c == ' ' || (!isalnum(c) && c != '-' && c != '_'))
			return (false);
	}
	return (true);
}

bool Server::isNickTaken(const std::string &nickname) const
{
	for (const auto &client : _clients)
	{
		if (client->getNick() == nickname)
			return (true);
	}
	return (false);
}

/**
 * This function processes the client's request to change their nickname. It performs the following checks:
 * 
 * 1. Ensures the provided nickname is not empty.
 * 2. Verifies that the nickname is valid according to the server's rules.
 * 3. Checks if the nickname is already taken by another client.
 * 
 * If all checks pass, the client's nickname is updated, and a reply is sent to both the client 
 * and the server with the successful change. If any checks fail, an appropriate error message is sent to the client.
 */


void Server::Nick(Client &client, const std::string &nickname)
{
	if (nickname.empty()) 
	{
		sendToClient(client, ERR_NONICKNAMEGIVEN(nickname));
		return;
	}

	if (!isValidNick(nickname))
    {
        sendToClient(client, ERR_ERRONEUSNICKNAME(client.getNick()));
        return;
    }

	if (isNickTaken(nickname))
	{
		sendToClient(client, ERR_NICKNAMEINUSE(client.getNick(), nickname));
		return;
	}

	sendToClient(client, RPL_NICKCHANGE(client.getNick(), client.getUserName(), client.getHostName(), nickname));
	client.setNick(nickname);
	client.setNickOK(true);
}
