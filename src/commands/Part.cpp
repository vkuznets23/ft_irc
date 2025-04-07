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
#include "../../inc/Client.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Message.hpp"

/**
 * Handles a client leaving a channel by sending the appropriate PART message
 * to all channel members and performing necessary updates to the channel's state.
 * 
 * 1. Checks if the channel exists. If not, it sends an error message to the client.
 * 2. Verifies if the client is in the channel. If not, it sends an error message.
 * 3. If the client leaving the channel is an operator, it checks if there are other operators in the channel.
 *    - If there are no other operators, it assigns the first available client as the new operator.
 *    - If the channel is empty after the client leaves, it removes the channel from the server's list of channels.
 * 4. Sends the PART message to all members of the channel to notify them of the client's departure.
 * 5. Removes the client from the channel's list of users.
 * 6. If the channel has no remaining users, it deletes the channel from the server.
 */

 void Server::Part(Client &client, const std::string &channels, const std::string &message)
 {
	 std::stringstream ss(channels);
	 std::string channelName;
 
	 while (std::getline(ss, channelName, ','))
	 {
		 auto it = _channels.find(channelName);
		 if (it == _channels.end())
		 {
			 sendToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
			 continue;
		 }
 
		 Channel &channel = it->second;
 
		 if (!channel.isClientInChannel(&client))
		 {
			 sendToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), channelName));
			 continue;
		 }
 
		 if (channel.isOperator(&client) && channel.countOperators() == 1)
		 {
			 std::vector<Client *> clients = channel.getUsers();
			 if (!clients.empty())
			 {
				 for (Client *potentialOp : clients)
				 {
					 if (potentialOp != &client)
					 {
						 channel.setOperator(potentialOp);
						 channel.unsetOperator(&client);
						 channel.displayChannelMessagePart(potentialOp, &client);
						 sendToClient(*potentialOp, RPL_YOUREOPER(potentialOp->getNick()));
						 break;
					 }
				 }
			 }
		 }
 
		 for (Client *member : channel.getUsers())
		 {
			 sendToClient(*member, RPL_PART(client.getNick(), client.getUserName(), client.getHostName(), channelName, message));
		 }
 
		 channel.removeClient(&client);
		 client.unsetJoinedChannel(&channel);
 
		 if (channel.getUsers().empty())
			 _channels.erase(it);
	 }
 }
 
