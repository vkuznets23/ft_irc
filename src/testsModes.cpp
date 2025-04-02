#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void testInviteOnlyMode()
{
    std::cout << "Running test: Invite-Only Mode (+i)..." << std::endl;

    // Create server, clients, and channel
    Server server;

    Client _operatorClient;
    _operatorClient.setNick("_operatorClient");

    Client regularClient;
    regularClient.setNick("regularClient");

    Channel channel("#test_channel");

    // Add operator client and regular client
    channel.addClient(&_operatorClient);
    channel.addClient(&regularClient);

    channel.setOperator(&_operatorClient);

    // Ensure operator status
    if (!channel.isOperator(&_operatorClient))
    {
        std::cout << "[FAIL] _operatorClient is not recognized as an operator!" << std::endl;
        return;
    }

    // Enable invite-only mode (+i)
    std::vector<std::string> params;
    channel.setParsedParameters(params);
    channel.setParsedModes("+i");
    server.executeModes(_operatorClient, &channel);

    if (channel.getInviteOnlyState())
        std::cout << "[PASS] Channel is now invite-only." << std::endl;
    else
        std::cout << "[FAIL] Invite-only mode was not set correctly!" << std::endl;

    std::cout << "Test completed.\n"
              << std::endl;
}

int main()
{
    testInviteOnlyMode();
    return 0;
}
