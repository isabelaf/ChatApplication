#include "MessageListWidgetItem.h"


MessageListWidgetItem::MessageListWidgetItem(AutoMessage * autoMessage) : 
	chatMessage{ autoMessage }
{
	this->setToolTip(QString::fromStdString(autoMessage->getMessage().getTimestampString()));

	this->widget = new MessageWidget{ *autoMessage };

	this->setSizeHint(this->widget->sizeHint());
}

MessageListWidgetItem::MessageListWidgetItem(UserMessage * userMessage, const bool isMainUser) : 
	chatMessage{ userMessage }
{
	this->setIcon(QIcon(QString::fromStdString(userMessage->getUser().getAvatarPhoto())));
	this->setToolTip(QString::fromStdString(userMessage->getMessage().getTimestampString()));

	this->widget = new MessageWidget{ *userMessage, isMainUser };

	this->setSizeHint(this->widget->sizeHint());
}