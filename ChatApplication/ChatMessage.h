#pragma once

#include "User.h"
#include "Message.h"


class ChatMessage
{
protected:
	Message message;
public:
	enum Type { AutoMessageType, UserMessageType };

	virtual ~ChatMessage() {}
	
	Message getMessage() const { return this->message; }

	virtual Type type() const = 0;
};


class AutoMessage : public ChatMessage
{
private:
	User aboutUser;
public:
	AutoMessage(const std::string & message, const User & aboutUser = User())
	{ 
		this->message = Message{ message };
		this->aboutUser = aboutUser;
	}

	User getAboutUser() const { return this->aboutUser; }

	Type type() const override { return AutoMessageType; }
};


class UserMessage : public ChatMessage
{
private:
	User user;
	bool isAttachment = false;
	std::string nameOfFileOnServer;

public:
	UserMessage() {}
	UserMessage(
		const User & _user, const std::string message, const bool _isAttachment = false, const std::string & _nameOfFileOnServer = std::string()) :
		user{ _user }, isAttachment{ _isAttachment }, nameOfFileOnServer{ _nameOfFileOnServer }
		{
			this->message = Message{ message };
		}

	User getUser() const { return this->user; }
	bool getIsAttachment() const { return this->isAttachment; }
	std::string getNameOfFileOnServer() const { return this->nameOfFileOnServer; }

	Type type() const override { return UserMessageType; }
};