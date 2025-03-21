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

void Server::Quit(Client &client, std::string message)
{
	std::istringstream stream(message);
	std::string command, reason;
	stream >> command >> reason;

	std::string quitMessage = "Client has disconnected";
	if (!reason.empty())
	{
		quitMessage = reason;
		if (quitMessage[0] == ':')
			quitMessage = quitMessage.substr(1);
	}
	//disconnectClient(client, quitMessage);
	std::cout << "[" + client.getUserName() + "] Client " << client.getNick() << " has disconnected" << std::endl;
}