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

#include <string>
#include <netinet/in.h>

enum clientState
{
    REGISTERING,
    REGISTERED,
    DISCONNECTED
};

class Client
{
	private:
		int 		_fd;
		int 		_state;
		sockaddr_in	_addr;
		std::string	_nick;
		std::string	_userName;
		std::string	_realName;
		bool		_passwdOK;
		bool		_nickOK;
		bool		_userNameOK;

	public:
		// constructors
		Client();
		Client(int fd, const sockaddr_in &client_addr);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		// methods
		void setFd(int value);
		int getFd() const;

		void setState(clientState state);
		clientState getState() const;

		void setUserName(std::string userName);
		std::string getUserName() const;

		void setRealName(std::string realName);
		std::string getRealName() const;

		void setNick(std::string nick);
		std::string getNick() const;
		
		void setPasswdOK(bool ok);
		bool getPasswdOK();

		void setNickOK(bool ok);
		bool getNickOK();

		void setUserNameOK(bool ok);
		bool getUserNameOK();
};
