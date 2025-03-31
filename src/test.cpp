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

void testInviteOnlyMode()
{
    Channel channel("invite_only_channel");

    assert(channel.getInviteOnlyState() == false);

    channel.setInviteOnly();
    assert(channel.getInviteOnlyState() == true);

    channel.unsetInviteOnly();
    assert(channel.getInviteOnlyState() == false);

    std::cout << "testInviteOnlyMode passed!" << std::endl;
}

void testPasswordMode()
{
    Channel channel("password_protected_channel");

    assert(channel.getChannelPassword() == "No password set");

    channel.setChannelPassword("securepass");
    assert(channel.getChannelPassword() == "securepass");

    channel.unsetChannelPassword();
    assert(channel.getChannelPassword() == "No password set");

    std::cout << "testPasswordMode passed!" << std::endl;
}

void testUserLimitMode()
{
    Channel channel("user_limit_channel");

    channel.setUserLimit(5);
    assert(channel.getUserLimit() == 5);

    channel.unsetUserLimit();
    assert(channel.getUserLimit() == -1);

    std::cout << "testUserLimitMode passed!" << std::endl;
}

void testTopicMode()
{
    Channel channel("topic_channel");

    Client client;
    client.setNick("user1");
    channel.addClient(&client);

    try
    {
        channel.setTopic(&client, "New Topic");
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "Client is not operator!");
    }

    std::cout << "testTopicMode passed!" << std::endl;
}

void testMultipleModes()
{
    Channel channel("multi_mode_channel");
    Client client1;
    client1.setNick("user1");
    channel.addClient(&client1);

    channel.setInviteOnly();
    channel.setChannelPassword("mypassword");
    channel.setUserLimit(10);

    // topic should be possible to set ONLY BY OPERATOR or if _isInviteOnly(false)
    try
    {
        channel.setTopic(&client1, "Topic for operators only");
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "Error while setting topic: " << e.what() << std::endl;
    }
    std::string mode = channel.getMode();

    assert(mode == "+i +k mypassword +l 10");

    channel.unsetInviteOnly();
    mode = channel.getMode();
    assert(mode == "+k mypassword +l 10");

    channel.unsetUserLimit();
    mode = channel.getMode();
    assert(mode == "+k mypassword");

    channel.unsetChannelPassword();
    mode = channel.getMode();
    assert(mode == "");

    std::cout << "testMultipleModes passed!" << std::endl;
}

void testChangeNick()
{
    Client client;
    client.setNick("user1");

    assert(client.getNick() == "user1");

    client.setNick("new_nick");
    assert(client.getNick() == "new_nick");

    std::cout << "testChangeNick passed!" << std::endl;
}

void testChannelUserLimit()
{
    Channel channel("full_channel");
    channel.setUserLimit(2);

    Client client1;
    client1.setNick("user1");
    channel.addClient(&client1);

    Client client2;
    client2.setNick("user2");
    channel.addClient(&client2);

    Client client3;
    client3.setNick("user3");

    try
    {
        channel.addClient(&client3);
        std::cerr << "Test failed, client3 should not be added!" << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "Channel is full!");
    }

    std::cout << "testChannelUserLimit passed!" << std::endl;
}

void testUnsetPassword()
{
    Channel channel("protected_channel");

    channel.setChannelPassword("password123");
    assert(channel.getChannelPassword() == "password123");

    channel.unsetChannelPassword();
    assert(channel.getChannelPassword() == "No password set");

    std::cout << "testUnsetPassword passed!" << std::endl;
}

int main()
{
    testAddAndRemoveClient();
    testChannelPassword();
    testUnsetPassword();
    testUserLimit();
    testSetTopic();
    testClientMethods();
    testInviteOnlyMode();
    testPasswordMode();
    testUserLimitMode();
    testTopicMode();
    testMultipleModes();
    testChangeNick();
    testChannelUserLimit();
    testUnsetPassword();

    std::cout
        << "All tests passed!" << std::endl;
    return 0;
}
