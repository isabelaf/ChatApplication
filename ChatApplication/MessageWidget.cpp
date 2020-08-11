#include "MessageWidget.h"
#include "Util.h"

#include <qhboxlayout>
#include <qfontmetrics.h>


MessageWidget::MessageWidget(AutoMessage autoMessage, const bool hasClickEvent)
{
	this->createMessageLabel(QString::fromStdString(autoMessage.getMessage().getMessageText()), "QLabel { color: blue; }", Qt::AlignCenter);

	if (hasClickEvent)
		this->hasClickEventAppearance();
}

MessageWidget::MessageWidget(UserMessage userMessage, const bool isMainUser)
{
	QString message, stylesheet;
	Qt::Alignment alignment;

	if (isMainUser)
	{
		message = QString::fromStdString(userMessage.getMessage().getMessageText());
		alignment = Qt::AlignRight;
	}
	else
	{
		message = QString::fromStdString(userMessage.getMessage().getMessageText());
		message.insert(
			this->findFirstParagraphInHtml(userMessage.getMessage().getMessageText()),
			"[ " + QString::fromStdString(userMessage.getUser().getName()) + " ]:</p>" +
			"<p style=\" margin - top:0px; margin - bottom:0px; margin - left:0px; margin - right:0px; -qt - block - indent:0; text - indent:0px; \">");
		alignment = Qt::AlignLeft;
	}

	if (userMessage.getIsAttachment())
	{
		stylesheet = "QLabel { color : blue; }";
	}
	else
	{
		stylesheet = "QLabel { color : green; }";
	}

	this->createMessageLabel(message, stylesheet, alignment);

	if (userMessage.getIsAttachment())
		this->hasClickEventAppearance();
}

void MessageWidget::createMessageLabel(const QString & message, const QString & stylesheet, const Qt::Alignment & alignment)
{
	this->messageLabel = new MessageLabel{ message };
	this->messageLabel->setAlignment(alignment);
	this->messageLabel->setStyleSheet(stylesheet);

	QHBoxLayout * layout = new QHBoxLayout{ this };
	layout->addWidget(this->messageLabel);
}

void MessageWidget::hasClickEventAppearance()
{
	this->hasClickEvent = true;

	this->setCursor(Qt::PointingHandCursor);

	this->messageLabel->setStyleSheet(this->messageLabel->styleSheet() + "QLabel { font: italic; }");
	this->messageLabel->setText(QString::fromStdString(UTIL::replaceFirstInString(
		this->messageLabel->text().toStdString(),
		"font-style:normal",
		"font-style:italic")));
}

int MessageWidget::findFirstParagraphInHtml(const std::string & html)
{
	int pos = html.find("<p");
	while (html[pos] != '>')
		pos++;
	return pos + 1;
}

void MessageWidget::enterEvent(QEvent * event)
{
	if (this->hasClickEvent)
	{
		this->messageLabel->setStyleSheet(QString::fromStdString(UTIL::replaceFirstInString(
			this->messageLabel->styleSheet().toStdString(), "italic", "bold")));

		this->messageLabel->setText(QString::fromStdString(UTIL::replaceFirstInString(
			this->messageLabel->text().toStdString(),
			"font-weight:400",
			"font-weight:bold")));
	}
}

void MessageWidget::leaveEvent(QEvent * event)
{
	if (this->hasClickEvent)
	{
		this->messageLabel->setStyleSheet(QString::fromStdString(UTIL::replaceFirstInString(
			this->messageLabel->styleSheet().toStdString(), "bold", "italic")));

		this->messageLabel->setText(QString::fromStdString(UTIL::replaceFirstInString(
			this->messageLabel->text().toStdString(),
			"font-weight:bold",
			"font-weight:400")));
	}
}

void MessageWidget::mousePressEvent(QMouseEvent * event)
{
	if (this->hasClickEvent)
	{
		emit clicked();
	}
}
