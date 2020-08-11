#include "Server.h"
#include "Util.h"

#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


std::string nameOfFileOnServer()
{
	time_t now = time(0);
	std::string name = "chat_" + UTIL::tmToString(*localtime(&now), "_", "", "", true);

	return name;
}

std::string SERVER::uploadFileToServer(const std::string & path, const std::string & name)
{
	std::error_code ec;

	fs::path whereFrom = path;
	fs::path whereTo = SERVER_LOC + name;

	if (fs::copy_file(whereFrom, whereTo, ec) == true)
			return name;
	
	SERVER::registerError(
		__FUNCTION__,
		__FILE__, 
		std::to_string(__LINE__), 
		std::vector<std::string>{ 
			"EC: " + ec.message(), 
			"fs::copy_file", 
			"Where from: " + whereFrom.string(), 
			"Where to: " + whereTo.string(),
			"Name of file: " + name });

	return std::string();
}

std::string SERVER::uploadChatAttachmentToServer(const std::string & path)
{
	std::string name = nameOfFileOnServer();

	name.insert(0, CHAT_ATT_LOC);
	name += "." + UTIL::split(path, '.').back();

	return SERVER::uploadFileToServer(path, name);
}

std::string SERVER::uploadUserPhotoToServer(const std::string & path, const std::string & userName, bool deleteIfExists)
{
	std::string name = USER_PIC_LOC + userName + "." + UTIL::split(path, '.').back();
	
	if (deleteIfExists)
	{
		SERVER::deleteFileFromServer(name);
	}

	return SERVER::uploadFileToServer(path, name);
}

std::string changeNameOfExistingFile(const std::string & currentName)
{
	std::string newName = currentName;
	size_t posStart = currentName.find('(');
	size_t posEnd = currentName.find('.');

	if (posStart == std::string::npos)
	{
		newName.insert(posEnd, " (1)");
	}
	else
	{
		std::string currentNumber;
		currentNumber.assign(currentName, posStart + 1, posEnd - posStart - 2);
		std::string newNumber = std::to_string((std::stoi(currentNumber) + 1));
		newName = UTIL::replaceFirstInString(newName, "(" + currentNumber + ")", "(" + newNumber + ")");
	}

	return newName;
}

bool SERVER::downloadFileFromServer(const std::string & nameOfFileOnServer, const std::string & locationPath)
{
	std::error_code ec;

	fs::path whereFrom = SERVER_LOC + nameOfFileOnServer;
	fs::path whereTo = locationPath + "\\" + UTIL::split(nameOfFileOnServer, '\\').back();

	while (fs::exists(whereTo, ec))
	{
		whereTo = changeNameOfExistingFile(whereTo.string());
	}
	//if (ec.value() != 0)
	//{
	//	registerError(
	//		__FUNCTION__,
	//		__FILE__,
	//		std::to_string(__LINE__),
	//		std::vector<std::string>{ 
	//			"EC: " + ec.message(), 
	//			"fs::exists", 
	//			"Where from: " + whereFrom.string(), 
	//			"Where to: " + whereTo.string(), 
	//			"Name of file on server: " + nameOfFileOnServer });
	//	return false;
	//}

	bool result = fs::copy_file(whereFrom, whereTo, ec);

	if (result == false)
	{
		registerError(
			__FUNCTION__,
			__FILE__,
			std::to_string(__LINE__),
			std::vector<std::string>{ 
				"EC: " + ec.message(), 
				"fs::copy_file", 
				"Where from: " + whereFrom.string(), 
				"Where to: " + whereTo.string(), 
				"Name of file on server: " + nameOfFileOnServer });
	}

	return result;
}

bool SERVER::deleteFileFromServer(const std::string & nameOfFileOnServer)
{
	std::error_code ec;

	fs::path whereFrom = SERVER_LOC + nameOfFileOnServer;
	
	bool result = fs::remove(whereFrom, ec);

	if (result == false)
	{
		SERVER::registerError(
			__FUNCTION__,
			__FILE__,
			std::to_string(__LINE__),
			std::vector<std::string>{ 
				"EC: " + ec.message(), 
				"fs::remove", 
				"Where from: " + whereFrom.string(), 
				"Name of file on server: " + nameOfFileOnServer });
	}

	return result;
}

void SERVER::eraseAllDataFromServer()
{
	std::string serverLoc = SERVER_LOC;
	std::vector<std::string> noDeleteFiles{ std::string(serverLoc + ERR_FILE_LOC), std::string(ADMINPASS_FILE_LOC) };

	for (auto & f : fs::recursive_directory_iterator(serverLoc, std::error_code()))
	{
		if (!(fs::is_directory(f, std::error_code()) || UTIL::substringsInString(f.path().string(), noDeleteFiles).size() != 0))
		{
			fs::remove(f, std::error_code());
		}
	}
}

bool SERVER::accessServer()
{
	const char * server = SERVER_LOC;
	wchar_t serverW[MAX_PATH];
	std::copy(server, server + lstrlenA(server) + 1, serverW);

	return (int)ShellExecute(NULL, L"open", serverW, NULL, NULL, SW_SHOWNORMAL) >= 32;
}

bool SERVER::openErrorsFile()
{
	const char * prog = "EXCEL.EXE";
	wchar_t progW[MAX_PATH];
	std::copy(prog, prog + lstrlenA(prog) + 1, progW);

	std::string fileLoc = std::string(SERVER_LOC) + std::string(ERR_FILE_LOC);
	const char * file = fileLoc.c_str();
	wchar_t fileW[MAX_PATH];
	std::copy(file, file + lstrlenA(file) + 1, fileW);

	return (int)ShellExecute(NULL, L"open", progW, fileW, NULL, SW_SHOWNORMAL) >= 32;
}

void SERVER::clearErrors()
{
	std::ofstream f(std::string(SERVER_LOC) + std::string(ERR_FILE_LOC));
	f << "DATE,TIME,STATUS,FUNCTION,FILE,LINE,ADDITIONAL INFO\n";
	f.close();
}

void SERVER::registerError(const std::string & function, const std::string & file, const std::string & line, const std::vector<std::string> & additionalInfo)
{
	time_t now = time(0);
	std::string time = UTIL::tmToString(*localtime(&now), ",");

	std::ofstream f(std::string(SERVER_LOC) + std::string(ERR_FILE_LOC), std::fstream::app);
	f << time << ",not checked," << function << "," << file << "," << line << "," << UTIL::removeCharacters(UTIL::concatenateStrings(additionalInfo, ","), std::vector<char>{ '\n', '\r' }) << "\n";
	f.close();
}