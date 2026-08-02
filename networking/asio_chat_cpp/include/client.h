#pragma once

#include <asio.hpp>
#include <safedeque.h>
#include <connection.h>

using asio::ip::tcp;

class Client
{
public:
	Client()
		: m_tempSocket(m_ioCntxt)
	{}

	~Client()
	{
		disconnect();
	}

    bool connect(const std::string& host = "127.0.0.1", uint16_t port = 12345, const std::string userName = "Uknown", bool onSeperateThread = false)
	{
		try {
			asio::async_connect(m_tempSocket, tcp::resolver(m_ioCntxt).resolve(host, std::to_string(port)),
				[this, userName](asio::error_code ec, asio::ip::tcp::endpoint _endpoint)
				{
					if (ec) {
						OnMessage("[CLIENT] Connection failed, " + ec.message(), nullptr);
						return;
					}

					m_connection = std::make_shared<Connection>(m_ioCntxt, std::move(m_tempSocket), m_msgsInQ, userName);

					m_connection->OnCloseCallback = std::bind(&Client::OnDisconnect, this, std::placeholders::_1);
					m_connection->OnMessageCallBack = std::bind(&Client::OnMessage, this, std::placeholders::_1, std::placeholders::_2);

					m_connection->open();

					OnConnect(_endpoint.address().to_string());
				});


			if (onSeperateThread)
				m_ioThread = std::thread([this]() { m_ioCntxt.run(); });
			else
				m_ioCntxt.run();
		}
		catch (const std::exception& ex)
		{
			OnMessage("[CLIENT] Error occured, " + std::string(ex.what()), nullptr);
			return false;
		}

		return true;
	}
	void disconnect() 
	{
		m_connection->close();

		m_ioCntxt.stop();

		if (m_ioThread.joinable())
			m_ioThread.join();
	}
	bool isConnected() { return true; }
	void send(const std::string& msg) 
	{
		m_connection->send(msg);
	}

protected:
	void printSafely(const std::string& msg)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		std::cout << msg << "\n";
	}

	virtual void OnMessage(const std::string& msg, std::shared_ptr<Connection>)
	{

	}

	virtual void OnConnect(const std::string& serverIP)
	{

	}

	virtual void OnDisconnect(std::shared_ptr<Connection> conn)
	{
		OnMessage("[CLIENT] You got disconnected!", nullptr);
	}

private:
	asio::io_context m_ioCntxt;
	std::thread m_ioThread;
	tcp::socket m_tempSocket;

	std::shared_ptr<Connection> m_connection;
	SafeDeque<std::string> m_msgsInQ;

	std::mutex m_mutex;
};
