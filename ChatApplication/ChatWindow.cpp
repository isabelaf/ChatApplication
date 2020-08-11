#include "ChatWindow.h"
#include "Appearance.h"
#include "MessageTextTextEdit.h"
#include "Util.h"
#include "Server.h"
#include "MessageListWidgetItem.h"
#include "EmojiView.h"

#include <qpushbutton.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextdocumentfragment>
#include <qtextdocument.h>
#include <qimagereader.h>
#include <qevent.h>

#include <algorithm>


ChatWindow::ChatWindow(const User & _user, ChatSession & _chatSession, QWidget * parent) :
	user{ _user }, chatSession { _chatSession }, QWidget(parent)
{
	ui.setupUi(this);
	Appearance::setMainWidgetAppearance(this);
	this->setWindowTitle(QString::fromStdString(this->user.getUserName()));

	this->connectToChat();
	this->chatSession.registerObserver(this);

	this->connectSignalsAndSlots();
}

void ChatWindow::connectSignalsAndSlots()
{
	connect(ui.SendMessage, &QPushButton::clicked, this, &ChatWindow::sendMessage);
	connect(ui.MessageText, &MessageTextTextEdit::enterPressed, this, &ChatWindow::sendMessage);
	connect(ui.MessageText, &MessageTextTextEdit::textChanged, this, &ChatWindow::SendMessageButtonBehaviour);
	connect(ui.SendFile, &QPushButton::clicked, this, &ChatWindow::sendAttachment);
	connect(ui.MessagesWindow, &QListWidget::itemSelectionChanged, this, &ChatWindow::downloadAttachment);
	connect(ui.SendEmoji, &QPushButton::clicked, this, &ChatWindow::sendEmoji);
	connect(ui.MessageText, &MessageTextTextEdit::expansionNeeded, this, &ChatWindow::expandMessageText);
	connect(ui.MessageText, &MessageTextTextEdit::decreaseNeeded, this, &ChatWindow::decreaseMessageText);
}

void ChatWindow::update()
{
	if (!this->isEnabled())
		return;

	ChatMessage * message = this->chatSession.getLastMessage();
	this->addMessageToList(message);
}

void ChatWindow::addMessageToList(ChatMessage * message)
{
	MessageListWidgetItem * m;

	if (message->type() == ChatMessage::AutoMessageType)
	{
		AutoMessage * autoMessage = (AutoMessage*)message;

		if (this->user.isBlockedUser(autoMessage->getAboutUser()) || this->user.isBlockedByUser(autoMessage->getAboutUser()))
			return;

		m = new MessageListWidgetItem{ autoMessage };
	}
	else if (message->type() == ChatMessage::UserMessageType)
	{
		UserMessage * userMessage = (UserMessage*)message;

		if (this->user.isBlockedUser(userMessage->getUser()))
		{
			//m = new MessageListWidgetItem{ new AutoMessage{ "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Segoe UI'; font-size:12pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"App Resources\\BlockedUserIcon.png\" width=\"20\" height=\"20\" /> This message belongs to a blocked user.</p></body></html>" } };
			m = new MessageListWidgetItem{ new AutoMessage{ "<!DOCTYPE html>\n<html>\n<body>\n<p><img src=\"App Resources\\BlockedUserIcon.png\" height=\"" + std::to_string(SIZE) + "\" width=\"" + std::to_string(SIZE) + "\"/> This message belongs to a blocked user.</p>\n</body></html>" } };
		}
		else if (this->user.isBlockedByUser(userMessage->getUser()))
		{
			m = new MessageListWidgetItem{ new AutoMessage{ "<!DOCTYPE html>\n<html>\n<body>\n<p><img src=\"App Resources\\BlockedUserIcon.png\" height=\"" + std::to_string(SIZE) + "\" width=\"" + std::to_string(SIZE) + "\"/> This message belongs to a user that blocked you.</p>\n</body></html>" } };
		}
		else
		{
			m = new MessageListWidgetItem{ userMessage, (this->user == userMessage->getUser()) };
		}
	}

	this->ui.MessagesWindow->addItem(m);
	this->ui.MessagesWindow->setItemWidget(m, m->getWidget());
	this->ui.MessagesWindow->scrollToBottom();

	connect((MessageWidget*)(m->getWidget()), &MessageWidget::clicked, this, [this, m]() { this->ui.MessagesWindow->setItemSelected(m, true); });
}

void ChatWindow::connectToChat()
{
	this->addMessageToList(this->chatSession.connectMessage());
	this->chatSession.addMessage(this->user.getUserName() + " connected to chat.", this->user);
}

void ChatWindow::disconnectFromChat()
{
	this->chatSession.addMessage(this->user.getUserName() + " has disconnected.", this->user);
}

void ChatWindow::disconnectSteps()
{
	if (this->executedDisconnectSteps == false)
	{
		this->chatSession.unregisterObserver(this);
		this->disconnectFromChat();

		SERVER::deleteFileFromServer(this->user.getAvatarPhoto());

		this->executedDisconnectSteps = true;
	}
}

void ChatWindow::enable()
{
	this->chatSession.addMessage(this->user.getUserName() + " has been enabled.", this->user);
	this->setEnabled(true);
	QMessageBox::information(this, "Chat Application", "You've been enabled again.");
}

void ChatWindow::disable()
{
	this->setEnabled(false);
	this->chatSession.addMessage(this->user.getUserName() + " has been disabled.", this->user);
	QMessageBox::information(this, "Chat Application", "The admin has temporarily disabled you. Please wait until you are enabled again.");
}

void ChatWindow::disconnect()
{
	this->setEnabled(false);
	this->disconnectSteps();
	QMessageBox::information(this, "Chat Application", "The admin has disconnected you.");
	this->close();
}

void ChatWindow::sendMessage()
{
	if (this->ui.MessageText->toPlainText().size() == 0)
		return;

	QString message = this->ui.MessageText->toHtml();

	bool isAttachment = false;
	std::string nameOfFileOnServer;

	if (this->ui.MessageText->isReadOnly() && this->ui.MessageText->toolTip().size() != 0)
	{
		this->ui.MessageText->setReadOnly(false);

		isAttachment = true;
		nameOfFileOnServer = this->ui.MessageText->toolTip().toStdString();
	}

	this->chatSession.addMessage(this->user, message.toStdString(), isAttachment, nameOfFileOnServer);

	this->ui.MessageText->clear();
	this->ui.MessageText->setToolTip("");
}

void ChatWindow::sendAttachment()
{
	QFileDialog dialog(this, QString(), "User\\Pictures");

	if (dialog.exec())
	{
		auto attch = dialog.selectedFiles();

		if (attch.size() == 0)
			return;

		std::string nameOfFileOnServer = SERVER::uploadChatAttachmentToServer(attch[0].toStdString());

		if (nameOfFileOnServer.size() == 0)
		{
			QMessageBox::warning(this, "Chat Application!", "Error! File cannot be uploaded!", QMessageBox::Ok);
			return;
		}

		this->ui.MessageText->clear();
		this->ui.MessageText->insertImage("App Resources\\AttachmentIcon.png", SIZE, SIZE);
		this->ui.MessageText->insertPlainText(attch[0].split("/").back());
		this->ui.MessageText->setReadOnly(true);
		this->ui.MessageText->setToolTip(QString::fromStdString(nameOfFileOnServer));
	}
}

void ChatWindow::downloadAttachment()
{
	auto selected = this->ui.MessagesWindow->selectedItems();
	
	if (selected.size() == 0)
		return;

	MessageListWidgetItem message = *(MessageListWidgetItem*)selected[0];

	if (message.getChatMessage()->type() == ChatMessage::UserMessageType && ((UserMessage*)message.getChatMessage())->getIsAttachment() == true)
	{
		QFileDialog dialog(this, QString(), "User\\Downloads");
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		dialog.setOption(QFileDialog::DontUseNativeDialog, true);
		dialog.setOption(QFileDialog::ShowDirsOnly, false);

		if (dialog.exec())
		{
			auto locationPath = dialog.selectedFiles();

			if (locationPath.size() == 0)
				return;

			if (SERVER::downloadFileFromServer(((UserMessage*)message.getChatMessage())->getNameOfFileOnServer(), locationPath[0].toStdString()) == false)
			{
				QMessageBox::warning(this, "Chat Application", "Error! Could not download file!", QMessageBox::Ok);
			}
			else
			{
				QMessageBox::information(this, "Chat Application", "File was downloaded!", QMessageBox::Ok);
			}
		}
	}

	this->ui.MessagesWindow->clearSelection();
}

void ChatWindow::sendEmoji()
{
	EmojiView * emojiView = new EmojiView{ this };
	connect(
		emojiView, 
		&EmojiView::clickSignal, 
		this, 
		[this, emojiView]() { 
		this->ui.MessageText->insertImage(emojiView->getSelectedEmoji(), SIZE, SIZE); });
	
	QPoint pos = this->mapToGlobal(QPoint{ this->ui.SendEmoji->geometry().topRight().x() - emojiView->width(), this->ui.SendEmoji->geometry().topRight().y() - emojiView->height() });
	emojiView->move(pos);

	emojiView->show();
}

void ChatWindow::SendMessageButtonBehaviour()
{
	if (this->ui.MessageText->toPlainText().size() == 0 && this->ui.MessageText->toHtml().indexOf("<img") == -1)
	{
		this->ui.SendMessage->setDisabled(true);
	}
	else
		this->ui.SendMessage->setDisabled(false);
}

void ChatWindow::expandMessageText()
{
	this->ui.MessagesWindow->setGeometry(
		this->ui.MessagesWindow->pos().x(),
		this->ui.MessagesWindow->pos().y(),
		this->ui.MessagesWindow->width(),
		this->ui.MessagesWindow->height() - (this->ui.MessageText->maximumHeight() - this->ui.MessageText->minimumHeight())
	);

	this->ui.MessageText->setGeometry(
		this->ui.MessageText->pos().x(),
		this->ui.MessageText->pos().y() - (this->ui.MessageText->maximumHeight() - this->ui.MessageText->minimumHeight()),
		this->ui.MessageText->width(),
		this->ui.MessageText->maximumHeight()
	);
}

void ChatWindow::decreaseMessageText()
{
	this->ui.MessagesWindow->setGeometry(
		this->ui.MessagesWindow->pos().x(),
		this->ui.MessagesWindow->pos().y(),
		this->ui.MessagesWindow->width(),
		this->ui.MessagesWindow->height() + (this->ui.MessageText->maximumHeight() - this->ui.MessageText->minimumHeight())
	);

	this->ui.MessageText->setGeometry(
		this->ui.MessageText->pos().x(),
		this->ui.MessageText->pos().y() + (this->ui.MessageText->maximumHeight() - this->ui.MessageText->minimumHeight()),
		this->ui.MessageText->width(),
		this->ui.MessageText->minimumHeight()
	);
}

void ChatWindow::closeEvent(QCloseEvent * event)
{
	this->disconnectSteps();
	QWidget::closeEvent(event);
}