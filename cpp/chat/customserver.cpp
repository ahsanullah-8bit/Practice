#include <common.h>
#include <connection.h>
#include <server.h>

class CustomServer : public Server
{
public:
    CustomServer(uint32_t port)
        : Server(port)
    {}

    bool OnClientConnect(std::shared_ptr<Connection> newConn) override
	{
		newConn->OnClientNameReceived = [this](std::shared_ptr<Connection> conn) {
				OnMessage("[SERVER] " + conn->clientName() + " with IP " + conn->address() + " entered the chat!", conn);
			};

		newConn->OnCloseCallback = [this](auto connection) {
				removeClient(connection);
                OnClientDisconnect(connection);
			};
		newConn->OnErrorCallback = [this](auto msg) { printSafely("[SERVER] Connection error, " + msg); };
		newConn->OnMessageCallBack = [this](const std::string& message, std::shared_ptr<Connection> conn)
			{
				OnMessage(("[" + conn->clientName() + "] " + message), conn);
			};
			
			//std::bind(&Server::OnMessage, this, std::placeholders::_1, std::placeholders::_2);

		return true;
	}

    void OnClientDisconnect(std::shared_ptr<Connection> client) override
	{
		OnMessage("[SERVER] " + client->clientName() + " disconnected!", client);
	}

	void OnMessage(const std::string& msg, std::shared_ptr<Connection> conn) override
	{
		broadcast(msg, conn);
		printSafely(msg);
	}
};

int main(int argc, char** argv) {
	int port = 12345;
	if (argc > 1)
	{
		if (argc > 2)
		{
			std::cerr << "Usage: ServerCLI <Port>" << std::endl;
			return EXIT_FAILURE;
		}
		else
			port = atoi(argv[1]);
	}

    CustomServer server(port);
    server.start(true);

	while (1)
	{
		std::string message;
		std::getline(std::cin, message);
		server.broadcast("[SERVER] " + message, nullptr);
	}
}
