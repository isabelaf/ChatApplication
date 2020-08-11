#pragma once

#include <string>
#include <vector>


class User
{
private:
	std::string userName, name, avatarPhoto;
	bool enabled = true;
	std::vector<User> blockedUsers;
	std::vector<User> blockedByUsers;

public:
	User() {}
	User(const std::string & _userName, const std::string & _name, const std::string & _avatarPhoto) :
		userName{ _userName }, name{ _name }, avatarPhoto{ _avatarPhoto } {}

	std::string getUserName() const { return this->userName; }
	std::string getName() const { return this->name; }
	std::string getAvatarPhoto() const { return this->avatarPhoto; }

	void blockUser(User * user) { 
		this->blockedUsers.push_back(*user); 
		user->blockedByUsers.push_back(*this); 
	}
	void unblockUser(User * user) {
		this->blockedUsers.erase(std::find(this->blockedUsers.begin(), this->blockedUsers.end(), *user)); 
		
		std::vector<User> blockedBy = user->blockedByUsers;
		blockedBy.erase(std::find(blockedBy.begin(), blockedBy.end(), *this));
		user->blockedByUsers = blockedBy;
	}
	bool isBlockedUser(const User & user) { return std::find(this->blockedUsers.begin(), this->blockedUsers.end(), user) != this->blockedUsers.end(); }
	bool isBlockedUser(const std::string & userName) { return std::find(this->blockedUsers.begin(), this->blockedUsers.end(), userName) != this->blockedUsers.end(); }
	bool isBlockedByUser(const User & user) { return std::find(this->blockedByUsers.begin(), this->blockedByUsers.end(), user) != this->blockedByUsers.end(); }
	bool isBlockedByUser(const std::string & userName) { return std::find(this->blockedByUsers.begin(), this->blockedByUsers.end(), userName) != this->blockedByUsers.end(); }
	std::vector<User> getBlockedUsers() const { return this->blockedUsers; }
	std::vector<User> getBlockedByUsers() const { return this->blockedByUsers; }

	bool isEnabled() const { return this->enabled; }
	void enable() { this->enabled = true; }
	void disable() { this->enabled = false; }

	friend bool operator==(const User & u1, const User & u2) { return (u1.userName == u2.userName); }
	friend bool operator==(const User & user, const std::string & userName) { return user.getUserName() == userName; }
};