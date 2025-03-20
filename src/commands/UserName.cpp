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

void Server::UserName(Client &client, const std::vector<std::string> &tokens, size_t &i)
{
	if (i + 4 >= tokens.size())
	{
		sendToClient(client, "461 USER :Not enough parameters");
		return;
	}

	std::string username = tokens[i + 1];
	std::string realname = tokens[i + 4];

	client.setUserName(username);
	client.setRealName(realname);

	sendToClient(client, "001 :Welcome to the IRC server, " + username);
}
