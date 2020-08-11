#include "ChatApplication.h"
#include "Appearance.h"
#include "Server.h"
#include "NewUserDialog.h"
#include "BlockUnblockUserDialog.h"

#include <qmessagebox.h>
#include <qevent.h>


ChatApplication::ChatApplication(ChatSession & _chatSession, QWidget * parent)
	: chatSession{ _chatSession }, QWidget(parent)
{
	ui.setupUi(this);
	Appearance::setMainWidgetAppearance(this);

	this->connectSignalsAndSlots();
}

void ChatApplication::connectSignalsAndSlots()
{
	connect(this->ui.NewUserButton, &QPushButton::clicked, this, &ChatApplication::createNewUserDialog);
	connect(this->ui.BlockUserButton, &QPushButton::clicked, this, &ChatApplication::createBlockUserDialog);
	connect(this->ui.UnblockUserButton, &QPushButton::clicked, this, &ChatApplication::createUnblockUserDialog);
}

void ChatApplication::closeEvent(QCloseEvent * event)
{
	//
	if (this->chatSession.getUsers().size() != 0)
	{
		event->ignore();
		return;
	}
	//

	SERVER::eraseAllDataFromServer();
	QWidget::closeEvent(event);
}

void ChatApplication::createNewUserDialog()
{
	NewUserDialog * newUserDialog = new NewUserDialog{ this->chatSession, this };

	if (newUserDialog->exec())
	{
		this->connectToChat(newUserDialog->newUserValues());
	}
}

void ChatApplication::createBlockUserDialog()
{
	BlockUnblockUserDialog * blockUserDialog = new BlockUnblockUserDialog{ this->chatSession, true, this };

	if (blockUserDialog->exec())
	{
		auto users = blockUserDialog->users();
		this->blockUser(users.first, users.second);
	}
}

void ChatApplication::createUnblockUserDialog()
{
	BlockUnblockUserDialog * unblockUserDialog = new BlockUnblockUserDialog{ this->chatSession, false, this };

	if (unblockUserDialog->exec())
	{
		auto users = unblockUserDialog->users();
		this->unblockUser(users.first, users.second);
	}
}

ChatWindow * ChatApplication::connectToChat(const std::vector<std::string> & values)
{
	User user{ values[0], values[1], values[2] };
	ChatWindow * w = new ChatWindow{ user, this->chatSession };
	w->show();

	return w;
}

void ChatApplication::blockUser(const std::string & userName, const std::string & blockedUserName)
{
	this->chatSession.blockUser(userName, blockedUserName);
}

void ChatApplication::unblockUser(const std::string & userName, const std::string & unblockedUserName)
{
	this->chatSession.unblockUser(userName, unblockedUserName);
}
