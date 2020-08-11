#pragma once

#include <QTextEdit>


class MessageTextTextEdit : public QTextEdit
{
	Q_OBJECT

private:
	bool isAtMaximumHeight = false;

public:
	MessageTextTextEdit(QWidget * parent);

	void insertImage(const QString & imageSource, const int h, const int w);
	void changeToEmoji();
	void sendResizeSignals();

	void textChangedEvents();

protected:
	void keyPressEvent(QKeyEvent * event) override;

	void insertFromMimeData(const QMimeData * source) override;

signals:
	void enterPressed();
	void expansionNeeded();
	void decreaseNeeded();
};
