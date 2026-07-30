#pragma once
#include <iostream>

enum class CustomMessages : uint32_t {
	ServerAccept,
	ServerDeny,
	UserInfo,
	ValidationHash,
	MessageAll,
	ServerMessage
};