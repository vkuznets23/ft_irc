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
		sendToClient(client, ERR_NONICKNAMEGIVEN(nickname));
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
