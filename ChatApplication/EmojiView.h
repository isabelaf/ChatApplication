#pragma once

#include <QWidget>
#include <qlistwidget.h>


class EmojiView : public QWidget
{
	Q_OBJECT

private:
	QStringList emojiList;
	QString selectedEmoji;

public:
	EmojiView(QWidget * parent = Q_NULLPTR);

	QString getSelectedEmoji() const { return this->selectedEmoji; }

	void init();
	void setEmojiList();

	static QStringList keyboardCodes();
	static std::tuple<QString, int, QString> replaceEmojiCode(const QString & html);
	static QString replaceAllEmojiCodes(const QString & html);

signals:
	void clickSignal();
};
