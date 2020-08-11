#include "UserValidator.h"
#include "Util.h"

#include <regex>
#include <cctype>


std::string UserValidator::validateUserName(const std::string & userName, ChatSession & chatSession)
{
	std::string errors = UserValidator::validateName(userName);

	if (userName.size() != 0 && !std::isalpha(userName[0]))
	{
		if (errors.size() != 0)
			errors += " ";

		errors += "Must start with a letter.";
	}

	if (UTIL::removeNotAlnumCharacters(userName, std::vector<char>{ '.', '_' }).size() != userName.size())
	{
		if (errors.size() != 0)
			errors += " ";

		errors += "Can only contain letters, digits, '.' and '_'.";
	}

	if (std::regex_search(userName, std::regex("[A-Z]")))
	{
		if (errors.size() != 0)
			errors += " ";

		errors += "Can only have lowercase letters.";
	}

	if (errors.size() == 0 && chatSession.isUser(userName))
		errors += "Username must be unique!";

	return errors;
}

std::string UserValidator::validateName(const std::string & name)
{
	std::string errors;

	if (UTIL::removeWhitespaceCharacters(name).size() < 3)
	{
		errors += "Must have at least 3 characters.";
	}
	if (name.size() > 15)
	{
		if (errors.size() != 0)
			errors += " ";

		errors += "Too many characters!";
	}

	return errors;
}
