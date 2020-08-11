#pragma once
#include "ChatSession.h"
#include "ChatWindow.h"

#include <QWidget>
#include "ui_ChatApplication.h"


class ChatApplication : public QWidget
{
	Q_OBJECT

private:
	ChatSession & chatSession;

public:
	ChatApplication(ChatSession & _chatSession, QWidget * parent = Q_NULLPTR);

	void connectSignalsAndSlots();

	void createNewUserDialog();
	void createBlockUserDialog();
	void createUnblockUserDialog();

	ChatWindow * connectToChat(const std::vector<std::string> & values);
	void blockUser(const std::string & userName, const std::string & blockedUserName);
	void unblockUser(const std::string & userName, const std::string & unblockedUserName);

protected:
	void closeEvent(QCloseEvent * event) override;

private:
	Ui::ChatApplication ui;
};
