#include <client.h>
#include <common.h>

class CustomClient : public Client
{
public:
    void send(const std::string& msg) {
        Client::send(msg);
    }

    void OnMessage(const std::string& msg, std::shared_ptr<Connection>) override
    {
        printSafely(msg);
    }

    void OnConnect(const std::string& serverIP) override
    {
		OnMessage("Connection successful to " + serverIP, nullptr);
    }

    void OnDisconnect(std::shared_ptr<Connection> conn) override
    {
        OnMessage("You got disconnected!", nullptr);
    }
};

int main(int argc, char** argv) {
    CustomClient client;

    std::string name;
    std::string ip = "127.0.0.1";
    int port = 12345;
    if (argc > 1)
    {
        if (argc != 4)
        {
			// I know we can take the whole address including port number and parse it, buttt....
            std::cerr << "Usage: ClientCLI.exe <User Name> <Server IP> <Server Port> \nNote: Provide name with no spaces" << std::endl;
            return EXIT_FAILURE;
        }
        else
        {
			name = argv[1];
			ip = argv[2];
            port = atoi(argv[3]);
        }
    }
    else 
    {
        std::cout << "Enter your name: ";
        std::getline(std::cin, name);
    }

    client.connect(ip, port, name, true);

    while (1)
    {
        std::string message;
        std::getline(std::cin, message);
        client.send(message);
    }
}
