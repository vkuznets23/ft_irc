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

void Server::Pass(Client &client, const std::vector<std::string> &tokens, size_t &i)
{
	if (i + 1 >= tokens.size())
	{
		sendToClient(client, "461 PASS :Not enough parameters");
		return;
	}

	std::string password = tokens[i + 1];

	if (password != _password)
	{
		sendToClient(client, "464 ERR_PASSWDMISMATCH :Password incorrect");
		return;
	}

	client.setPasswdOK(true);
	sendToClient(client, "NOTICE :Password accepted");
}
