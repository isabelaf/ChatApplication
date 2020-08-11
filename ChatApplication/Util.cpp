#include "Util.h"

#include <sstream>
#include <cctype>


std::vector<std::string> UTIL::split(const std::string & str, const char separator)
{
	std::vector<std::string> fields;
	std::string field;
	std::stringstream stringStr(str);

	while (std::getline(stringStr, field, separator))
	{
		fields.push_back(field);
	}

	return fields;
}

std::string UTIL::replaceFirstInString(const std::string & str, const std::string & what, const std::string & with)
{
	if (what == with)
		return str;

	std::string newString = str;
	size_t pos = str.find(what);

	if (pos != std::string::npos)
	{
		newString.replace(pos, what.size(), with);
	}

	return newString;
}

std::string UTIL::replaceAllInString(const std::string & str, const std::string & what, const std::string & with)
{
	if (what == with)
		return str;

	std::string newString = str;

	size_t pos = newString.find(what);

	while (pos != std::string::npos)
	{
		newString.replace(pos, what.size(), with);
		pos = newString.find(what, pos + with.size());
	}

	return newString;
}

std::vector<int> UTIL::substringsInString(const std::string & str, const std::vector<std::string> & substrings)
{
	std::vector<int> result;

	for (int i = 0; i < substrings.size(); i++)
	{
		if (str.find(substrings[i]) != std::string::npos)
			result.push_back(i);
	}

	return result;
}

std::string UTIL::removeWhitespaceCharacters(const std::string & str)
{
	std::string result;

	for (auto c : str)
	{
		if (!std::isspace(c))
		{
			result += c;
		}
	}

	return result;
}

std::string UTIL::removeNotAlnumCharacters(const std::string & str, const std::vector<char> & allowedCharacters)
{
	std::string result;

	for (auto c : str)
	{
		if (isalnum(c) || std::find(allowedCharacters.begin(), allowedCharacters.end(), c) != allowedCharacters.end())
			result += c;
	}

	return result;
}

std::string UTIL::removeCharacters(const std::string & str, const std::vector<char> & toRemove)
{
	std::string result;

	for (auto c : str)
	{
		if (std::find(toRemove.begin(), toRemove.end(), c) == toRemove.end())
			result += c;
	}

	return result;
}

std::string UTIL::concatenateStrings(const std::vector<std::string> & strings, const std::string & separator)
{
	std::string concatenated;

	for (int i = 0; i < strings.size(); i++)
	{
		if (strings[i].size() != 0)
		{
			if (i != 0)
				concatenated += separator;
			concatenated += strings[i];
		}
	}

	return concatenated;
}

std::string UTIL::tmToString(const tm & time, const std::string & separator, const std::string & dateSeparator, const std::string & timeSeparator, const bool useSeconds)
{
	std::stringstream t;

	int day = time.tm_mday, month = time.tm_mon + 1, year = time.tm_year + 1900, hour = time.tm_hour, minute = time.tm_min;

	if (day < 10)
		t << "0";
	t << day << dateSeparator;
	if (month < 10)
		t << "0";
	t << month << dateSeparator;
	t << year;

	t << separator;

	if (hour < 10)
		t << "0";
	t << hour << timeSeparator;
	if (minute < 10)
		t << "0";
	t << minute;

	if (useSeconds)
	{
		int seconds = time.tm_sec;
		t << timeSeparator;
		if (seconds < 10)
			t << "0";
		t << seconds;
	}

	return t.str();
}

std::string UTIL::upperString(const std::string & str)
{
	std::string result = str;

	for (auto & c : result)
	{
		c = std::toupper(c);
	}

	return result;
}

std::string UTIL::lowerString(const std::string & str)
{
	std::string result = str;

	for (auto & c : result)
	{
		c = std::tolower(c);
	}

	return result;
}

std::string UTIL::htmlEscaped(const std::string & str)
{
	std::string html = str;
	std::vector<std::pair<std::string, std::string>> escapedCh{ std::make_pair("&", "&amp;"), std::make_pair("<", "&lt;"), std::make_pair(">", "&gt;"), std::make_pair("\"", "&quot;") };

	for (auto c : escapedCh)
	{
		html = UTIL::replaceAllInString(html, c.first, c.second);
	}

	return html;
}
