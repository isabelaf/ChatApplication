#include "EmojiView.h"
#include "Appearance.h"
#include "Util.h"

#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qtextdocument.h>

#include <fstream>


EmojiView::EmojiView(QWidget * parent) : QWidget(parent)
{
	this->setEmojiList();
	this->init();
}

void EmojiView::init()
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);

	QGridLayout * layout = new QGridLayout{ this };

	QStringList codesList = EmojiView::keyboardCodes();
	int emojiNo = 0;

	for (int r = 0; r < this->emojiList.size() / 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			QPushButton * emoji = new QPushButton{};
			emoji->setCursor(Qt::PointingHandCursor);
			emoji->setIcon(QIcon(this->emojiList[emojiNo]));
			emoji->setIconSize(QSize(SIZE, SIZE));
			emoji->setFixedSize(emoji->iconSize());
			emoji->setStyleSheet("QPushButton { background : transparent; }");
			connect(emoji, &QPushButton::clicked, this, [this, emojiNo]() {
				this->selectedEmoji = this->emojiList[emojiNo];
				emit clickSignal();
			});
			if (emojiNo < codesList.size())
				emoji->setToolTip(codesList[emojiNo]);

			layout->addWidget(emoji, r + 1, c + 1);

			emojiNo++;
		}
	}

	this->adjustSize();
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void EmojiView::setEmojiList()
{
	QString loc = "App Resources\\Emojis\\";

	for (int i = 0; i < 15; i++)
		this->emojiList.push_back(loc + "emoji (" + QString::number(i + 1) + ").png");
}

QStringList EmojiView::keyboardCodes()
{
	QStringList codesList;
	std::string code;

	std::fstream f("App Resources\\Emojis\\emoji_codes.txt");

	while (std::getline(f, code))
	{
		codesList.push_back(QString::fromStdString(code));
	}

	f.close();

	return codesList;
}

std::tuple<QString, int, QString> EmojiView::replaceEmojiCode(const QString & html)
{
	QStringList codesList = EmojiView::keyboardCodes();

	if (codesList.size() == 0)
		return std::make_tuple(QString(), -1, QString());

	for (int i = 0; i < codesList.size(); i++)
	{
		QString code = codesList[i]; code.toHtmlEscaped();

		QTextDocument doc{};
		doc.setHtml(html);

		int index = doc.toPlainText().indexOf(code, 0, Qt::CaseInsensitive);

		if (index != -1)
		{
			QString replaced = doc.toHtml().replace(code, "", Qt::CaseInsensitive);
			QString emoji = "App Resources\\Emojis\\emoji (" + QString::number(i + 1) + ").png";
			return std::make_tuple(replaced, index, emoji);
		}
	}

	return std::make_tuple(QString(), -1, QString());
}

QString EmojiView::replaceAllEmojiCodes(const QString & html)
{
	QString newHtml = html;
	QStringList codesList = EmojiView::keyboardCodes();

	if (codesList.size() == 0)
		return QString();

	for (int i = 0; i < codesList.size(); i++)
	{
		newHtml = newHtml.replace(codesList[i].toHtmlEscaped(), "<img src='App Resources\\Emojis\\emoji (" + QString::number(i + 1) + ").png' height='" + QString::number(SIZE) + "' width='" + QString::number(SIZE) + "'>", Qt::CaseInsensitive);
	}

	if (html != newHtml)
		return newHtml;

	return QString();
}
