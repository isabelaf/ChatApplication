#pragma once
#include "User.h"
#include "ChatSession.h"

#include <vector>


class UserValidator
{
public:
	static std::string validateUserName(const std::string & userName, ChatSession & chatSession);
	static std::string validateName(const std::string & name);
};