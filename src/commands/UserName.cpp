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

/**
 * This function is responsible for assigning a username and a real name to a client 
 * when they send the USER command. It performs the necessary validation to ensure 
 * that the command is used correctly and that the client has not already set their 
 * username.
 */

void Server::UserName(Client &client, const std::string &username, const std::string &realname)
{
    if (client.getUserNameOK())
    {
        sendToClient(client, ERR_ALREADYREGISTRED());
        return;
    }

	client.setUserName(username);
	client.setRealName(realname);
	client.setUserNameOK(true);
}
