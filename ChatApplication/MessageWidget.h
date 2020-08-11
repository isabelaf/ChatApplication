#pragma once
#include "ChatMessage.h"
#include "MessageLabel.h"

#include <qpushbutton.h>


class MessageWidget : public QWidget
{
	Q_OBJECT

private:
	MessageLabel * messageLabel;
	bool hasClickEvent = false;

public:
	MessageWidget() {}
	MessageWidget(AutoMessage autoMessage, const bool hasClickEvent = false);
	MessageWidget(UserMessage userMessage, const bool isMainUser);

	void createMessageLabel(const QString & message, const QString & stylesheet, const Qt::Alignment & alignment);
	void hasClickEventAppearance();
	int findFirstParagraphInHtml(const std::string & html);

protected:
	void enterEvent(QEvent * event) override;
	void leaveEvent(QEvent * event) override;
	void mousePressEvent(QMouseEvent * event) override;

signals:
	void clicked();
};