#include "BlockUnblockUserDialog.h"
#include "Appearance.h"

#include <qmessagebox.h>


BlockUnblockUserDialog::BlockUnblockUserDialog(ChatSession & _chatSession, const bool block, QWidget * parent)
	: chatSession{ _chatSession }, QDialog(parent)
{
	ui.setupUi(this);
	Appearance::setMainWidgetAppearance(this, DIALOG_FONT_SIZE);

	this->block = block;

	this->init();
	this->connectSignalsAndSlots();
}

void BlockUnblockUserDialog::init()
{
	QString Step2_Title, BlockUnblockButton_Text;

	if (this->block)
	{
		Step2_Title = "Username of user you want to block:";
		BlockUnblockButton_Text = "Block user";
	}
	else
	{
		Step2_Title = "Username of user you want to unblock:";
		BlockUnblockButton_Text = "Unblock user";
	}

	this->ui.Step2->setTitle(Step2_Title);
	this->ui.BlockUnblockButton->setText(BlockUnblockButton_Text);
}

void BlockUnblockUserDialog::connectSignalsAndSlots()
{
	connect(this->ui.ContinueButton, &QPushButton::clicked, this, &BlockUnblockUserDialog::ContinueButtonHandler);
	connect(this->ui.BlockUnblockButton, &QPushButton::clicked, this, &BlockUnblockUserDialog::BlockUnblockButtonHandler);
}

void BlockUnblockUserDialog::ContinueButtonHandler()
{
	if (this->chatSession.isUser(this->ui.UserNameValue->text().toStdString()))
	{
		this->ui.Step1->setEnabled(false);
		this->ui.Step2->setEnabled(true);
	}
	else
	{
		QMessageBox::warning(this, "Chat Application", "Username doesn't exist!");
	}
}

void BlockUnblockUserDialog::BlockUnblockButtonHandler()
{
	if (this->ui.BlockedUnblockedUserNameValue->text() == this->ui.UserNameValue->text() || 
		!this->chatSession.isUser(this->ui.BlockedUnblockedUserNameValue->text().toStdString()))
	{
		QMessageBox::warning(this, "Chat Application", "Invalid username!");
	}
	else if (this->block && this->chatSession.getUser(this->ui.UserNameValue->text().toStdString()).isBlockedUser(this->ui.BlockedUnblockedUserNameValue->text().toStdString()))
	{
		QMessageBox::warning(this, "Chat Application", "User already blocked!");
	}
	else if (!this->block && !this->chatSession.getUser(this->ui.UserNameValue->text().toStdString()).isBlockedUser(this->ui.BlockedUnblockedUserNameValue->text().toStdString()))
	{
		QMessageBox::warning(this, "Chat Application", "User hasn't been blocked by you!");
	}
	else
	{
		QString confirmationQ;
		if (this->block)
			confirmationQ = "Are you sure you want to block this user?";
		else
			confirmationQ = "Are you sure you want to unblock this user?";

		if (QMessageBox(QMessageBox::Question, "Chat Application", confirmationQ, QFlags<QMessageBox::Button>{ QMessageBox::Yes, QMessageBox::Abort }).exec() == QMessageBox::Yes)
		{
			QDialog::accept();
		}
	}
}

std::pair<std::string, std::string> BlockUnblockUserDialog::users()
{
	return std::make_pair(this->ui.UserNameValue->text().toStdString(), this->ui.BlockedUnblockedUserNameValue->text().toStdString());
}
