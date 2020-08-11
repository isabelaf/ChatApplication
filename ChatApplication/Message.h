#pragma once
#include "Util.h"

#include <string>
#include <ctime>


class Message
{
private:
	tm timestamp;
	std::string messageText;

public:
	Message() {}
	Message(const std::string & _messageText) : messageText{ _messageText }
	{
		time_t now = time(0);
		this->timestamp = *localtime(&now);
	}

	std::string getMessageText() const { return this->messageText; }
	tm getTimestamp() const { return this->timestamp; }

	std::string getTimestampString() const
	{	
		return "[" + UTIL::tmToString(this->timestamp) + "]";
	}
};