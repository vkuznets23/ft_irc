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

bool Server::isNickTaken(const std::string &nickname) const
{
	for (const auto &client : _clients)
	{
		if (client->getNick() == nickname)
			return (true);
	}
	return (false);
}

void Server::Nick(Client &client, const std::string &nickname)
{
	if (nickname.empty()) 
	{
		sendToClient(client, "431 ERR_NONICKNAMEGIVEN :No nickname given");
		return;
	}

	if (isNickTaken(nickname))
	{
		sendToClient(client, "433 ERR_NICKNAMEINUSE " + nickname + " :Nickname is already in use");
		return;
	}

	client.setNick(nickname);
	client.setNickOK(true);
	sendToClient(client, "NICK " + nickname);
}
