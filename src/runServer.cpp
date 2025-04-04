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
#include "../inc/Message.hpp"

// signals handling

volatile sig_atomic_t serverIsRunning = 1;

void handle_sig(int signum)
{
	switch (signum)
	{
	case SIGSEGV:
		std::cerr << "Segmentation fault occurred! Exiting." << std::endl;
		break;
	case SIGINT:
	case SIGTERM:
	case SIGQUIT:
		std::cout << "\nShutting down server..." << std::endl;
		break;
	default:
		return;
	}
	serverIsRunning = 0;
}

void setupSignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = handle_sig;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	int signals[] = {SIGINT, SIGTERM, SIGQUIT, SIGSEGV};
	for (int sig : signals)
	{
		sigaction(sig, &sa, nullptr);
	}
}

// Socket Creation socket()-> Binding bind() -> Listening listen() -> Accepting Connections accept() -> Reading data read()
int Server::createServerSocket()
{
	// create a socket IPv4 according to TCP protocol, 0 is by default
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
	}

	// set reuse_address thingy to run IRC at the same address
	// @server_fd - socket descriptor
	// @SOL_SOCKET - general settings
	// @SO_REUSEADDR - option that let reuse address and port if in time_wait state (prevent mistake  "Address already in use" )
	// &opt - pointer to the value (1 - turn on, 0 - turn off)
	int reuseAddressOption = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseAddressOption, sizeof(reuseAddressOption)) == -1)
	{
		close(server_fd);
		throw std::system_error(errno, std::generic_category(), "setsockopt failed: Unable to set SO_REUSEADDR");
	}
	return server_fd;
}

void Server::bindAndListen(int server_fd)
{
	sockaddr_in server_addr = {};
	server_addr.sin_family = AF_INET;         // Set the address family to IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available IP addresses
	server_addr.sin_port = htons(_port);      // Set the port to listen

	// @server_fd - socket descriptor
	// @&server_addr - pointer to a struct we created sockaddr_in
	// size of sockaddr_in struct
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Bind failed");
		close(server_fd);
		throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
	}

	// start listening incoming things
	if (listen(server_fd, MAX_CLIENTS) == -1)
	{
		perror("Listening failed");
		close(server_fd);
		throw std::system_error(errno, std::generic_category(), "Listen failed");
	}

	std::cout << "Server is listening on port " << _port << "..." << std::endl;
}

void Server::acceptNewConnection(int server_fd)
{
	sockaddr_in client_addr = {};
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

	if (client_fd < 0)
	{
		perror("accept");
		close(server_fd);
		return;
	}

	std::cout << "Client connected" << std::endl;
	_clients.push_back(new Client(client_fd, client_addr));
	_clients.back()->setState(REGISTERING);
}

void Server::cleanupResources(int server_fd)
{
	for (auto &client : _clients)
	{
		close(client->getFd());
		delete client;
	}
	_clients.clear();
	close(server_fd);
}

void Server::runServer()
{
	setupSignalHandlers();
	int server_fd = createServerSocket();
	bindAndListen(server_fd);

	std::vector<struct pollfd> fds;
	while (serverIsRunning)
	{
		fds.clear();
		fds.push_back({server_fd, POLLIN, 0}); // Monitor the server socket for new connections

		for (const auto &client : _clients)
		{
			fds.push_back({client->getFd(), POLLIN, 0}); // Monitor active clients
		}
		int poll_result = poll(fds.data(), fds.size(), -1);
		if (poll_result == -1)
		{
			if (errno == EINTR)
			{
				if (!serverIsRunning) // not sure about this one
					break;
				continue;
			}
			perror("Poll failed");
			break;
		}

		// Check if there are incoming connections on the server socket
		// @POLLIN flag shows that there is data in the socket
		if (fds[0].revents & POLLIN)
		{
			acceptNewConnection(server_fd);
		}

		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[BUFFER_SIZE];
				ssize_t bytes_read = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
				if (bytes_read <= 0)
				{
					std::cout << "Client disconnected" << std::endl;
					close(fds[i].fd);
					_clients.erase(_clients.begin() + i - 1);
				}
				else
				{
					buffer[bytes_read] = '\0';
					std::string message(buffer);
					handleClientMessage(*_clients[i - 1], message);
			}
			}
		}
	}

	// clean up
	cleanupResources(server_fd);
}