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

    std::cout << "[" + client.getUserName() + "] Client " << client.getNick() << " has disconnected: " << quitMessage << std::endl;

    // Fermer le socket du client
    close(client.getFd());

    // Supprimer le client de la liste des clients
    for (auto it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == &client)
        {
            _clients.erase(it);
            delete &client;
            break;
        }
    }
}