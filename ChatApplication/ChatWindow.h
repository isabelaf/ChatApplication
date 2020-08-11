#pragma once
#include "Observer.h"
#include "ChatSession.h"

#include <QWidget>
#include "ui_ChatWindow.h"


class ChatWindow : public QWidget, public Observer
{
	Q_OBJECT

private:
	User user;
	ChatSession & chatSession;

	bool executedDisconnectSteps = false;
	
public:
	ChatWindow(const User & _user, ChatSession & _chatSession, QWidget * parent = Q_NULLPTR);

	void connectSignalsAndSlots();

	void update() override;

	User getUser() { return this->user; }

	User * getPointerToUser() { return &this->user; }

	void addMessageToList(ChatMessage * message);

	void connectToChat();
	void disconnectFromChat();

	void disconnectSteps();

	void enable();
	void disable();
	void disconnect();

	void sendMessage();
	void sendAttachment();
	void downloadAttachment();
	void sendEmoji();

	void SendMessageButtonBehaviour();

	void expandMessageText();
	void decreaseMessageText();

protected:
	void closeEvent(QCloseEvent * event) override;

private:
	Ui::ChatWindow ui;
};
