#pragma once

#include <asio.hpp>
#include "safedeque.h"
#include "connection.h"
#include <memory>

using asio::ip::tcp;

class Server 
{
public:
	Server(uint16_t port)
		: m_acceptor(m_ioCntxt, tcp::endpoint(tcp::v4(), port))
	{}

	~Server() 
	{
		stop();
	}

	bool start(bool onSeperateThread = false)
	{
		asio::co_spawn(m_ioCntxt, 
			[this] () -> asio::awaitable<void> 
			{
				try
				{
					while (true) 
					{
						auto socket = co_await m_acceptor.async_accept(asio::use_awaitable);
						auto newConn = std::make_shared<Connection>(m_ioCntxt, std::move(socket), m_msgsInQ);

                        if (OnClientConnect(newConn)) {
							newConn->open();
							m_connections.push_back(newConn	);	
						}
						/*else {
							OnMessage("Client " + newConn->address() + " got rejected!");
						}*/
					}
				}
				catch (const std::exception& ex)
				{
					OnMessage("[SERVER] Accept connection error, " + std::string(ex.what()), nullptr);
				}
			}, 
			asio::detached);

		// We won't get the host's IP, but anyway...
		OnMessage("[SERVER] Server has started on IP " + m_acceptor.local_endpoint().address().to_string() + " port " + std::to_string(m_acceptor.local_endpoint().port()), nullptr);

		if (onSeperateThread)
			m_ioThread = std::thread([this] { m_ioCntxt.run(); });
		else
			m_ioCntxt.run();

		return true;
	}

	void broadcast(const std::string& msg, std::shared_ptr<Connection> exceptThisClient)
	{
		// We'll have to use a copy of m_connections because of iteration not being thread safe
		// I couldn't think of any other solution as of yet.
		auto clients = m_connections.getDeque();

		for (auto client : clients)
			if (client != exceptThisClient)
				client->send(msg);
	}

	void removeClient(std::shared_ptr<Connection> conn)
	{
		m_connections.remove(conn);
	}

	void stop()
	{
		m_ioCntxt.stop();

		if (m_ioThread.joinable())
			m_ioThread.join();

		OnMessage("[SERVER] Server stopped!", nullptr);
	}

public:
    virtual bool OnClientConnect(std::shared_ptr<Connection> newConn)
	{
		return true;
	}

    virtual void OnClientDisconnect(std::shared_ptr<Connection> client)
	{

	}

	virtual void OnMessage(const std::string& msg, std::shared_ptr<Connection>)
	{
		printSafely(msg);
	}

protected:
	void printSafely(const std::string& msg)
	{
		std::lock_guard<std::mutex> l(m_printMtx);
		std::cout << msg << "\n";
	}

protected:
	asio::io_context m_ioCntxt;
	std::thread m_ioThread;
	tcp::acceptor m_acceptor;

	SafeDeque<std::shared_ptr<Connection>> m_connections;
	SafeDeque<std::string> m_msgsInQ;

	std::mutex m_printMtx;
};
