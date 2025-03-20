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

#include "Server.hpp"

Server::Server() {}

Server::Server(int port, std::string password) : _port(port), _password(password) {}

Server::Server(const Server &o)
{
    this->_port = o._port;
    this->_password = o._password;
}

Server &Server::operator=(const Server &o)
{
    if (this != &o)
    {
        this->_port = o._port;
        this->_password = o._password;
    }
    return *this;
}

Server::~Server() {}