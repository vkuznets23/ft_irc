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

void Server::Cap(Client &client, const std::vector<std::string>& tokens, int index)
{
	std::string response;
	if (tokens[index + 1] == "LS")
	{
		response = "CAP * LS :multi-prefix sasl";
		sendToClient(client, response);
	}
	else if (tokens[index + 1] == "REQ")
	{
		response = ":localhost CAP " + client.getNick() + " ACK :multi-prefix";
		sendToClient(client, response);
	}
	else if (tokens[index + 1] == "END")
	{
		std::cout << "END sended" << std::endl;
		if (!client.getPasswdOK())
		{
			sendToClient(client, " :This server requires a password. Please send: PASS <password>");
			close(client.getFd());
			//removeClient(client.getFd());
			return;
		}
		else if (!client.getNickOK())
		{
			sendToClient(client, "NOTICE * :This server requires a user nickname. Please send: NICK <nickname>");
			close(client.getFd());
			//removeClient(client.getFd());
			return;
		}
		else if (!client.getUserNameOK())
		{
			sendToClient(client, "NOTICE * :This server requires a user user name. Please send: User <username username localhost :Name>");
			close(client.getFd());
			//removeClient(client.getFd());
			return;
		}
		else
		{
			client.setState(REGISTERED);
			sendToClient(client, "001 : Welcome " + client.getNick() + " to the ft_irc network.");
		}	
	}
	else
		std::cerr << "Invalid CAP message" << std::endl;
}