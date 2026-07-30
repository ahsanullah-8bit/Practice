// License: Well, credit goes to javidx9 for the architecture and design in this file.
// I'm making a little change, however...

#pragma once
#include <iostream>
#include <vector>
#include <memory>

template <typename T>
struct MessageHeader {
	T id{};
    std::size_t size = 0;
};

template <typename T>
struct Message {
	MessageHeader<T> header{};
	std::vector<uint8_t> body;

	std::size_t size() {
		return sizeof(header) + body.size();
	}

	friend std::ostream& operator<<(std::ostream& os, const Message<T>& msg) {
		os << "ID: " << int(msg.header.id) << " size: " << msg.header.size();
		return os;
	}

	template <typename DataType>
	friend Message<T> operator << (Message<T>& msg, const DataType& data) {
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed.");

		std::size_t i = msg.body.size();
		msg.body.resize(msg.body.size() + sizeof(DataType));

        memcpy(msg.body.data() + i, &data, sizeof(DataType));

		msg.header.size = msg.size();

		return msg;
	}

	friend Message<T>& operator<<(Message<T>& msg, const std::string& data) {
		std::size_t i = msg.body.size();
		msg.body.resize(msg.body.size() + data.size() + sizeof(std::size_t));

		std::size_t dataSize = data.size();
		std::memcpy(msg.body.data() + i, &dataSize, sizeof(std::size_t));
		std::memcpy(msg.body.data() + i + sizeof(std::size_t), data.data(), data.size());

		msg.header.size = msg.body.size();
		return msg;
	}
	
	template <typename DataType>
	friend Message<T> operator>>(Message<T>& msg, DataType& data) {

		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled.");

		std::size_t i = msg.body.size() - sizeof(DataType);
        memcpy(&data, msg.body.data() + i, sizeof(DataType));

		msg.body.resize(i);

		msg.header.size = msg.size();

		return msg;
	}

	friend Message<T>& operator>>(Message<T>& msg, std::string& data) {
		std::size_t i = msg.body.size() - sizeof(std::size_t);
		std::size_t dataSize;
		std::memcpy(&dataSize, msg.body.data() + i, sizeof(std::size_t));
		msg.body.resize(i);

		i = msg.body.size() - dataSize;
		data.resize(dataSize);
		std::memcpy(&data[0], msg.body.data() + i, dataSize);
		msg.body.resize(i);

		msg.header.size = msg.body.size();
		return msg;
	}
};

class Connection;

template <typename T>
struct OwnedMessage {
	std::shared_ptr<Connection>  remote = nullptr;
	Message<T> message;

	friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg) {
		os << msg.message;
		return os;
	}
};