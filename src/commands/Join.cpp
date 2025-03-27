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

bool Server::checkChannelType(Client &client, Channel &channel, const std::string &channelName, const std::string &password)
{
    if (channel.getChannelType() == INVITE_ONLY && !channel.isOperator(&client))
    {
        sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
        return false;
    }

    if (channel.getChannelType() == PRIVATE && !channel.isClientInChannel(&client))
    {
        sendToClient(client, ERR_BANNEDFROMCHAN(client.getNick(), channelName));
        return false;
    }

    if (channel.getChannelType() == MODERATED && !channel.isOperator(&client))
    {
        sendToClient(client, ERR_INVITEONLYCHAN(client.getNick(), channelName));
        return false;
    }

    if (!channel.getChannelPassword().empty() && channel.getChannelPassword() != password)
    {
        sendToClient(client, ERR_BADCHANNELKEY(client.getNick(), channelName));
        return false;
    }

    return true;
}


/**
 * This function handles a client joining one or more channels. 
 * It checks if the channels exist, validates mode restrictions, adds the client to the channel, 
 * and sends appropriate messages to the client and other members of the channel.
 */

void Server::Join(Client &client, std::string &channels, std::string &password)
{
	if (channels.empty())
	{
		sendToClient(client, "\r\n");
		return;
	}

	std::stringstream ss(channels);
	std::string channelName;

	while (std::getline(ss, channelName, ','))
	{
		bool channelExists = false;
		for (auto &channelPair : _channels)
		{
			Channel &channel = channelPair.second;

			if (channel.getChannelName() == channelName)
			{
				channelExists = true;
				if (!checkChannelType(client, channel, channelName, password))
                    return;
				
				channel.addClient(client);

				std::string namesList = "";
				for (Client *clientsIn : channel.getClients())
					namesList.append(clientsIn->getNick() + " ");

				for (Client *member : channel.getClients())
				{
					sendToClient(*member, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), channelName));
					sendToClient(*member, RPL_NAMREPLY(client.getNick(), channelName));
					sendToClient(*member, RPL_ENDOFNAMES(client.getNick(), channelName));
				}

				std::string topic = channel.getTopic();
				if (!topic.empty())
					sendToClient(client, RPL_NOTOPIC(channelName));
				else
					sendToClient(client, RPL_TOPIC(channelName, topic));

				sendToClient(client, RPL_AVAILABLECMD(client.getNick()));

				return;
			}
		}

		if (!channelExists)
		{
			Channel *newChannel = new Channel(channelName);
			newChannel->setTimestamp();
			newChannel->setChannelType(PUBLIC);

			if (!password.empty())
			{
				newChannel->setChannelPassword(password);
			}

			newChannel->addClient(client);
			newChannel->setOperator(&client);

			_channels[channelName] = *newChannel;

			sendToClient(client, RPL_JOIN(client.getNick(), client.getUserName(), client.getHostName(), channelName));
			sendToClient(client, RPL_NAMREPLY(client.getNick(), channelName));
			sendToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));

			return;
		}
	}
}
