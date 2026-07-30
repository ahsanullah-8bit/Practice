#pragma once

#include "safedeque.h"
#include <asio.hpp>
#include <asio/system_error.hpp>
#include <iostream>
#include <memory>

using asio::ip::tcp;

// T: Type of message
class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(asio::io_context& ioCntx, tcp::socket socket, SafeDeque<std::string>& inQ, const std::string& clientName = "")
		: m_ioCntxt(ioCntx), 
		m_socket(std::move(socket)), 
		m_timer(ioCntx),
		m_msgsInQ(inQ),
		m_clientName(clientName)
	{
		m_timer.expires_at(std::chrono::steady_clock::time_point::max());

		// Write the Client name
		if (!clientName.empty())
			send("/userName " + clientName);
	}

	~Connection()
	{
		close();
	}

	bool open()
	{
		if (!isConnected())
			return false;

		asio::co_spawn(m_ioCntxt, read(), asio::detached);
		asio::co_spawn(m_ioCntxt, write(), asio::detached);

		return true;
	}

	void close()
	{
		if (!m_isClosing.exchange(true))
			if (isConnected())
			{
				m_timer.cancel();
				m_socket.shutdown(asio::socket_base::shutdown_both);
				m_socket.close();

				if (OnCloseCallback)
					OnCloseCallback(shared_from_this());
			}
	}

	void send(const std::string& msg)
	{
		m_msgsOutQ.push_back(msg + "\n");
		m_timer.cancel_one();
	}

	bool isConnected()
	{
		return m_socket.is_open();
	}

	std::string address()
	{
		return m_socket.remote_endpoint().address().to_string();
	}

	std::string clientName() { return m_clientName; }

private:
	asio::awaitable<void> read()
	{
		try
		{
			asio::streambuf m_readBffr;
			while (m_socket.is_open()) 
			{
				asio::error_code ec;
				co_await asio::async_read_until(m_socket, m_readBffr, "\n", asio::use_awaitable);

				std::istream is(&m_readBffr);
				std::string messageIn;
				std::getline(is, messageIn);

				if (messageIn.starts_with("/userName"))
				{
					auto loc = messageIn.find_first_of(" ");
					m_clientName = messageIn.substr(loc + 1);
					OnClientNameReceived(shared_from_this());
				}
				else
					//std::cout << m_clientName << ": " << messageIn << "\n";
					OnMessageCallBack(messageIn, shared_from_this());
			}
		}
		catch (const std::exception& ex)
		{
			//std::cout << "Exception: " << ex.what() << "\n";
			close();
		}
	}

	asio::awaitable<void> write()
	{
		try
		{
			while (!m_isClosing && m_socket.is_open()) {
				if (m_msgsOutQ.empty())
				{
					asio::error_code ec;
					co_await m_timer.async_wait(asio::redirect_error(asio::use_awaitable, ec));

					if (ec && ec != asio::error::operation_aborted)
					{
						//std::cout << "Exception: " << ec.message() << "\n";
						throw asio::system_error(ec);
					}

					continue;
				}
				
				co_await asio::async_write(m_socket, asio::buffer(m_msgsOutQ.front(), m_msgsOutQ.front().size()), asio::use_awaitable);
				m_msgsOutQ.pop_front();
			}
		}
		catch (const std::exception& ex)
		{
			//std::cout << "Exception: " << ex.what() << "\n";
			close();
		}
	}

public:
	std::function<void(std::shared_ptr<Connection>)> OnCloseCallback;
	std::function<void(const std::string&)> OnErrorCallback;
	std::function<void(const std::string& message, std::shared_ptr<Connection>)> OnMessageCallBack;
	std::function<void(std::shared_ptr<Connection>)> OnClientNameReceived;

private:
	asio::io_context& m_ioCntxt;
	tcp::socket m_socket;
	asio::steady_timer m_timer;

	SafeDeque<std::string>& m_msgsInQ;
	SafeDeque<std::string> m_msgsOutQ;

	std::string m_clientName;
	std::atomic<bool> m_isClosing{ false };
};