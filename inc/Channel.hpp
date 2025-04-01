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

#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <vector>
#include <iostream>
#include <ctime>
#include <set>

class Client;

enum ChannelType
{
	PUBLIC,
	INVITE_ONLY,
	PRIVATE,
	MODERATED
};

class Channel
{
	private:
		std::string				_channelName;
		std::string				_channelPassword;
		int						_userLimit;
		std::string				_topic;
		bool					_topicOperatorsOnly;
		std::vector<Client *>	_userList;
		std::vector<Client *>	_operatorList;
		ChannelType				_type;
		std::string				_timestamp;
		Client*					operatorClient = nullptr;
		std::set<std::string>	_invitedUsers;

	public:
		// Constructors
		Channel();
		Channel(std::string name);
		Channel(const Channel &o);
		Channel &operator=(const Channel &o);
		~Channel();

		// Getters & Setters
		std::string	getChannelName() const;
		std::string	getChannelPassword() const;
		void		setChannelPassword(const std::string &password);
		void		setUserLimit(int limit);
		int			getUserLimit() const;
		void		setTimestamp();
		std::string	getTimestamp() const;
		void		setTopic(Client *client, const std::string &topic);
		std::string	getTopic() const;
		bool		isTopicRestricted() const;
		void		setChannelType(ChannelType type);
		ChannelType	getChannelType() const;

		// Client Methods
		void					addClient(Client &client);
		void					removeClient(Client *client);
		bool					isClientInChannel(Client *client);
		std::vector<Client *>	getClients() const;

		// Operator Methods
		void	setOperator(Client *client);
		Client	*getOperator() const;
		bool	isOperator(Client *client) const;

		// Display message to all clients in the channel
		void displayChannelMessage(Client &sender, const std::string &message);

		//Invite
		void	addInvite(const std::string &nickname);
		bool	isInvited(const std::string &nickname) const;
		void	removeInvite(const std::string &nickname);
};
