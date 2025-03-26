#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

void testAddAndRemoveClient()
{
    Channel channel("general");

    Client client1;
    client1.setNick("user1");

    Client client2;
    client2.setNick("user2");

    channel.addClient(&client1);
    assert(channel.isClientInChannel(&client1) == true);

    channel.addClient(&client2);
    assert(channel.isClientInChannel(&client2) == true);

    channel.removeClient(&client1);
    assert(channel.isClientInChannel(&client1) == false);

    std::cout << "testAddAndRemoveClient passed!" << std::endl;
}

void testChannelPassword()
{
    Channel channel("private_channel");

    // std::cout << "Password: " << channel.getChannelPassword() << std::endl;

    // Set the channel password
    channel.setChannelPassword("securepass");
    assert(channel.getChannelPassword() == "securepass");

    std::cout << "testChannelPassword passed!" << std::endl;
}

void testUserLimit()
{
    Channel channel("limited_channel");

    // Set the user limit to 3
    channel.setUserLimit(3);

    // Check that the user limit is set correctly
    assert(channel.getUserLimit() == 3);

    std::cout << "testUserLimit passed!" << std::endl;
}

void testSetTopic()
{
    Channel channel("topic_channel");
    Client client1;
    client1.setNick("user1");

    // Add client to the channel
    channel.addClient(&client1);

    // Setting the topic should throw an exception if client is not operator
    try
    {
        channel.setTopic(&client1, "New Topic");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "Client is not operator!"); // Client should not be able to set the topic without operator rights
    }

    std::cout << "testSetTopic passed!" << std::endl;
}

void testClientMethods()
{
    // Create a client instance
    Client client;
    client.setNick("user1");
    client.setUserName("username1");
    client.setRealName("Real Name");

    // Check client’s nick, username, and real name
    assert(client.getNick() == "user1");
    assert(client.getUserName() == "username1");
    assert(client.getRealName() == "Real Name");

    // Test setting and getting state
    client.setState(REGISTERED);
    assert(client.getState() == REGISTERED);

    std::cout << "testClientMethods passed!" << std::endl;
}

void testChannelType()
{
    Channel channel("test_channel");
    // assert(channel.getChannelType() == PUBLIC);

    channel.setChannelType(PRIVATE);
    assert(channel.getChannelType() == PRIVATE);

    channel.setChannelType(MODERATED);
    assert(channel.getChannelType() == MODERATED);

    channel.setChannelType(INVITE_ONLY);
    assert(channel.getChannelType() == INVITE_ONLY);

    std::cout << "testChannelType passed!" << std::endl;
}

int main()
{
    testAddAndRemoveClient();
    testChannelPassword();
    testUserLimit();
    testSetTopic();
    testClientMethods();
    testChannelType();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
