<h1 align="center">
	ft_irc
</h1>
<p align="center">
	<b><i>Recreating IRC, one command at a time... </i></b><br>
</p>

### The Challenge
The goal of this project was to recreate a basic IRC server from scratch, implementing fundamental features and mimicking the behavior of a real IRC network. We had to handle both the server-side and client-side functionalities to simulate a communication platform where users can connect, join channels, and send messages. The features we had to implement included:

Server:
- Accept incoming client connections.
- Handle multiple clients simultaneously using forks or threads.
- Handle basic IRC protocol commands (NICK, USER, JOIN, PRIVMSG, etc.).
- Create and manage channels where clients can join and leave.
- Broadcast messages to all users in a channel.
- Implement user authentication and handle disconnections gracefully.
- Implement proper error handling for invalid commands or server issues.

Client:
- Implement a simple IRC client that can connect to the server.
- Handle user input, including sending messages, joining channels, and setting a nickname.
- Display server messages, channel messages, and error messages to the user.
- Implement basic input/output handling with commands like /nick, /join, and /msg.
- Support for multiple channels.
- Handling of private messages.
- Server messages such as PING and PONG.

We were required to follow the RFC for IRC to ensure compatibility with the basic commands and behaviors of a typical IRC network.


### Installation
#### Clone the repository:
``` 
git clone https://github.com/YourUsername/ft_irc.git
cd ft_irc
make
```
#### Run Server
```
./ircserv <port> <password>
```

#### Run Client
```
./irssi
/connect localhost <port> <password>
```

#### Some commands to try:

Once the server and client are running, you can try the following commands to test the functionality:

1. Set a nickname:
```
/nick <nickname>
```
2. Join a channel:
```
/join #channel_name
```
3. Send a message to the channel:
```
/msg #channel_name <message>
```
4. Send a private message to a user:
```
/msg <nickname> <message>
```
5. Set a topic:
```
/topic #channel_name <topic>
```
6. Set the channel to be private (invite-only):
```
/mode #channel_name +i
```
7. Leave a channel:
```
/part #channel_name
```

*to exit the IRC client:*
```
/quit
```

*Note: Make sure the server is running before connecting any clients.*

------
<p align="center">
	Made by Viktoriia Kuznetsova (@vkuznets) and Juliette Mouette (@jmouette) <br>
</p>
