#include "ChatSession.h"
#include "ChatWindow.h"


void ChatSession::registerObserver(Observer * obs)
{
	Subject::registerObserver(obs);
	this->users.push_back((*(ChatWindow*)obs).getPointerToUser());
}

void ChatSession::unregisterObserver(Observer * obs)
{
	Subject::unregisterObserver(obs);
	this->users.erase(std::find(this->users.begin(), this->users.end(), (*(ChatWindow*)obs).getPointerToUser()));
}

void ChatSession::addMessage(const User & user, const std::string & message, const bool isAttatchment, const std::string & nameOfFileOnServer)
{
	this->messages.push_back(new UserMessage(user, message, isAttatchment, nameOfFileOnServer));
	this->notify();
}

void ChatSession::addMessage(const std::string & message, const User & aboutUser)
{
	this->messages.push_back(new AutoMessage(message, aboutUser));
	this->notify();
}

bool ChatSession::isUser(const std::string & userName) const
{
	for (auto u : this->users)
	{
		if (*u == userName)
			return true;
	}
	return false;
}

std::vector<User> ChatSession::getUsers() const
{
	std::vector<User> users;
	std::transform(this->users.begin(), this->users.end(), std::back_inserter(users), [](User * user) { return *user; });

	return users;
}

User ChatSession::getUser(const std::string & userName) const
{
	std::vector<User> users = this->getUsers();
	return *std::find(users.begin(), users.end(), userName);
}

AutoMessage * ChatSession::connectMessage()
{
	if (this->users.size() == 0)
		return new AutoMessage("No connected users.");

	std::vector<std::string> connectedUsers;
	std::transform(this->users.begin(), this->users.end(), std::back_inserter(connectedUsers), [](User * user) { return user->getUserName(); });

	return new AutoMessage("Connected users: " + UTIL::concatenateStrings(connectedUsers, ", ") + ".");
}

void ChatSession::enableUser(const User & user)
{
	for (int i = 0; i < this->users.size(); i++)
	{
		if (*this->users[i] == user)
		{
			this->users[i]->enable();
			((ChatWindow*)this->observers[i])->enable();
			break;
		}
	}
}

void ChatSession::disableUser(const User & user)
{
	for (int i = 0; i < this->users.size(); i++)
	{
		if (*this->users[i] == user)
		{
			this->users[i]->disable();
			((ChatWindow*)this->observers[i])->disable();
			break;
		}
	}
}

void ChatSession::disconnectUser(const User & user)
{
	for (int i = 0; i < this->users.size(); i++)
	{
		if (*this->users[i] == user)
		{
			//((ChatWindow*)this->observers[i])->close();
			((ChatWindow*)this->observers[i])->disconnect();
			break;
		}
	}
}

void ChatSession::blockUser(const std::string & userName, const std::string & blockedUserName)
{
	int userPos = -1, blockedUserPos = -1;

	for (int i = 0; i < this->users.size(); i++)
	{
		if (*this->users[i] == userName)
		{
			userPos = i;
		}
		else if (*this->users[i] == blockedUserName)
		{
			blockedUserPos = i;
		}
		if (userPos != -1 && blockedUserPos != -1)
			break;
	}

	this->users[userPos]->blockUser(this->users[blockedUserPos]);
}

void ChatSession::unblockUser(const std::string & userName, const std::string & unblockedUserName)
{
	int userPos = -1, unblockedUserPos = -1;

	for (int i = 0; i < this->users.size(); i++)
	{
		if (*this->users[i] == userName)
		{
			userPos = i;
		}
		else if (*this->users[i] == unblockedUserName)
		{
			unblockedUserPos = i;
		}
		if (userPos != -1 && unblockedUserPos != -1)
			break;
	}

	this->users[userPos]->unblockUser(this->users[unblockedUserPos]);
}
