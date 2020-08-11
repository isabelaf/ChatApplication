#pragma once
#include "ChatMessage.h"
#include "MessageWidget.h"

#include <qlistwidget.h>


class MessageListWidgetItem : public QListWidgetItem
{
	//Q_OBJECT

private:
	ChatMessage * chatMessage;
	MessageWidget * widget;

public:
	MessageListWidgetItem() {}
	MessageListWidgetItem(AutoMessage * autoMessage);
	MessageListWidgetItem(UserMessage * userMessage, const bool isMainUser);

	ChatMessage * getChatMessage() const { return this->chatMessage; }
	QWidget * getWidget() { return this->widget; }
};