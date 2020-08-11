#pragma once

#include <vector>
#include <string>


namespace UTIL
{
	std::vector<std::string> split(const std::string & str, const char separator);

	std::string replaceFirstInString(const std::string & str, const std::string & what, const std::string & with);

	std::string replaceAllInString(const std::string & str, const std::string & what, const std::string & with);

	std::vector<int> substringsInString(const std::string & str, const std::vector<std::string> & substrings);

	std::string removeWhitespaceCharacters(const std::string & str);

	std::string removeNotAlnumCharacters(const std::string & str, const std::vector<char> & allowedCharacters = std::vector<char>{});

	std::string removeCharacters(const std::string & str, const std::vector<char> & toRemove);

	std::string concatenateStrings(const std::vector<std::string> & strings, const std::string & separator);

	std::string tmToString(const tm & time, const std::string & separator = "; ", const std::string & dateSeparator = ".", const std::string & timeSeparator = ":", const bool useSeconds = false);

	std::string upperString(const std::string & str);

	std::string lowerString(const std::string & str);

	std::string htmlEscaped(const std::string & str);
}