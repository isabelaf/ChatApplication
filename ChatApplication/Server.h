#pragma once

#include <string>
#include <vector>

#define SERVER_LOC "Server\\"
#define CHAT_ATT_LOC "Chat Attachments\\"
#define USER_PIC_LOC "User Photos\\"
#define ERR_FILE_LOC "server_errors.csv"
#define ADMINPASS_FILE_LOC "Server\\admin_pass.txt"


namespace SERVER
{
	std::string uploadFileToServer(const std::string & path, const std::string & name);

	std::string uploadChatAttachmentToServer(const std::string & path);

	std::string uploadUserPhotoToServer(const std::string & path, const std::string & userName, bool deleteIfExists = false);

	bool downloadFileFromServer(const std::string & nameOfFileOnServer, const std::string & locationPath);

	bool deleteFileFromServer(const std::string & nameOfFileOnServer);

	void eraseAllDataFromServer();

	bool accessServer();

	bool openErrorsFile();

	void clearErrors();

	void registerError(
		const std::string & function,
		const std::string & file,
		const std::string & line,
		const std::vector<std::string> & additionalInfo);
}