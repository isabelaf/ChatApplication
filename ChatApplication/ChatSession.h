#pragma once
#include "ChatMessage.h"
#include "Observer.h"

#include <string>


class ChatSession : public Subject
{
private:
	std::vector<User*> users;
	std::vector<ChatMessage*> messages;

public:
	ChatSession() {}

	void registerObserver(Observer * obs) override;

	void unregisterObserver(Observer * obs) override;

	void addMessage(const User & user, const std::string & message, const bool isAttatchment = false, const std::string & nameOfFileOnServer = std::string());

	void addMessage(const std::string & message, const User & aboutUser = User());

	std::vector<ChatMessage*> getMessages() const { return this->messages; }

	ChatMessage * getLastMessage() const { return this->messages.back(); }

	bool isUser(const std::string & userName) const;

	std::vector<User> getUsers() const;

	User getUser(const std::string & userName) const;

	AutoMessage * connectMessage();

	void enableUser(const User & user);

	void disableUser(const User & user);

	void disconnectUser(const User & user);

	void blockUser(const std::string & userName, const std::string & blockedUserName);

	void unblockUser(const std::string & userName, const std::string & unblockedUserName);
};