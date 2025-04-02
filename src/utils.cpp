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

#include "../inc/Server.hpp"

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == std::string::npos)
		return ("");

	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	return (str.substr(first, (last - first + 1)));
}

void Server::sendToClient(Client client, const std::string &message)
{
	std::string response = message + "\r\n";
	if (send(client.getFd(), response.c_str(), response.size(), 0) == -1)
		perror("Error sending message to client.");
	else
		std::cout << ">> " << message << std::endl;
}

std::vector<std::string> Server::split(const std::string &str)
{
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;

	while (iss >> token)
		tokens.push_back(token);

	return (tokens);
}