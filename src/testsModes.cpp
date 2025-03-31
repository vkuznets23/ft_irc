#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void testInviteOnlyMode()
{
    std::cout << "Running test: Invite-Only Mode (+i)..." << std::endl;

    // Create server, clients, and channel
    Server server;

    Client operatorClient;
    operatorClient.setNick("operatorClient");

    Client regularClient;
    regularClient.setNick("regularClient");

    Channel channel("#test_channel");

    // Add operator client and regular client
    channel.addClient(&operatorClient);
    channel.addClient(&regularClient);

    channel.setOperator(&operatorClient);

    // Ensure operator status
    if (!channel.isOperator(&operatorClient))
    {
        std::cout << "[FAIL] operatorClient is not recognized as an operator!" << std::endl;
        return;
    }

    // Enable invite-only mode (+i)
    std::vector<std::string> params;
    channel.setParsedParameters(params);
    channel.setParsedModes("+i");
    server.executeModes(operatorClient, &channel);

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
