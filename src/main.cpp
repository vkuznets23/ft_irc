/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viktoria <viktoria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:37:16 by viktoria          #+#    #+#             */
/*   Updated: 2025/03/18 15:37:46 by viktoria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int checkValidPort(std::string port)
{
    int portNo;
    if (port.length() > 5)
        return 0;
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
            return 0;
    }
    portNo = std::stoi(port);
    if (portNo < 1024 || portNo > 49151)
        return 0;
    return portNo;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>";
        return (0);
    }
    int port = checkValidPort(argv[1]);
    Server server(port, argv[2]);
    try
    {
        server.runServer();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return 0;
}