#include "MessageLabel.h"
#include "Appearance.h"

#include <qfontmetrics.h>
#include <qevent.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qtextdocument.h>


MessageLabel::MessageLabel(const QString & text, QWidget * parent)
	: QLabel(parent)
{
	this->setWrappedText(text);
	
	this->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
	this->setOpenExternalLinks(true);

	this->setContextMenuPolicy(Qt::NoContextMenu);
}

QString MessageLabel::getSelectedText()
{
	QString selectedText = this->selectedText();

	selectedText = MessageLabel::removeWrapChars(selectedText, this->wordWrapChars);

	return selectedText;
}

void MessageLabel::setWrappedText(const QString & text)
{
	this->setText(MessageLabel::wrapText(text, this->wordWrapChars));
}

QString MessageLabel::wrapText(const QString & text, std::vector<int> & wordWrapChars)
{
	if (text.indexOf(QRegExp("<!DOCTYPE HTML", Qt::CaseInsensitive)) != -1)
		return MessageLabel::wrapHtmlText(text, wordWrapChars);
	else
		return MessageLabel::wrapPlainText(text, wordWrapChars);
}

QString MessageLabel::wrapHtmlText(const QString & html, std::vector<int> & wordWrapChars)
{
	QTextDocument doc{};
	doc.setHtml(html);

	QStringList wrappedTextLines = MessageLabel::wrapPlainText(doc.toPlainText(), wordWrapChars).split('\n');
	std::vector<QString> images = MessageLabel::getImagesFromHtml(html);

	doc.clear();
	doc.setDefaultFont(Appearance::makeFont());

	QString wrappedHtml = doc.toHtml();
	
	int img = 0;
	for (auto line : wrappedTextLines)
	{
		int indx = line.indexOf((QChar)65532);
		while (indx != -1)
		{
			line = line.replace(indx, 1, "");
			line = line.insert(indx, images[img++]);
			indx = line.indexOf((QChar)65532, indx);
		}
		wrappedHtml.insert(wrappedHtml.indexOf("</body>"), "<p style=\" margin - top:0px; margin - bottom:0px; margin - left:0px; margin - right:0px; -qt - block - indent:0; text - indent:0px; \">" + line + "</p>");
	}

	return wrappedHtml;
}

QString MessageLabel::wrapPlainText(const QString & plainText, std::vector<int> & wordWrapChars)
{
	QString wrappedText;

	auto paragraphs = plainText.split(QRegExp("\n"));

	QFontMetrics fontMetrics{ Appearance::makeFont() };

	for (auto paragraph : paragraphs)
	{
		QString aux;
		int i = 0, lastSpace = -1;
		while (i < paragraph.size())
		{
			if (fontMetrics.width(aux) + fontMetrics.width(paragraph[i]) < MAX_TEXT_WIDTH)
			{
				aux.append(paragraph[i]);
				if (paragraph[i].isSpace() || paragraph[i] == '-')
				{
					lastSpace = i;
				}
				i++;
			}
			else
			{
				if (lastSpace >= 0)
				{
					paragraph[lastSpace] = '\n';
					i = lastSpace + 1;

					wordWrapChars.push_back(lastSpace);
				}
				else
				{
					paragraph.insert(i, "-\n");

					wordWrapChars.push_back(i);

					i = i + 2;
				}
				lastSpace = -1;
				aux.clear();
			}
		}
		if (wrappedText.size() != 0)
			wrappedText += "\n";
		wrappedText += paragraph;
	}

	return wrappedText;
}

QString MessageLabel::removeWrapChars(const QString & text, const std::vector<int> & wordWrapChars)
{
	QString unwrappedText = text;

	int deleted = 0;
	for (int i = 0; i < wordWrapChars.size(); i++)
	{
		if (unwrappedText[wordWrapChars[i] - deleted] == '\n' || unwrappedText[wordWrapChars[i] - deleted] == (QChar)8233)
		{
			unwrappedText.replace(wordWrapChars[i] - deleted, 1, " ");
		}
		else if (unwrappedText[wordWrapChars[i] - deleted] == '-')
		{
			unwrappedText.replace(wordWrapChars[i] - deleted, 2, "");
			deleted += 2;
		}
	}

	return unwrappedText;
}

QString MessageLabel::removeExtraText(const QString & text)
{
	std::vector<int> wordWrapChars;

	QString wrappedText = MessageLabel::wrapText(text, wordWrapChars);
	
	return MessageLabel::removeWrapChars(wrappedText.split('\n').mid(0, MAX_ROWS_NUMBER - 1).join("\n"), wordWrapChars);
}

int MessageLabel::numberOfRows(const QString & text)
{
	if (text.indexOf(QRegExp("<!DOCTYPE HTML", Qt::CaseInsensitive)) != -1)
		return MessageLabel::numberOfRowsHtmlText(text);
	else
		return MessageLabel::numberOfRowsPlainText(text);
}

int MessageLabel::numberOfRowsHtmlText(const QString & html)
{
	return MessageLabel::wrapHtmlText(html).mid(html.indexOf("<p")).split('\n').size();
}

int MessageLabel::numberOfRowsPlainText(const QString & plainText)
{
	return MessageLabel::wrapPlainText(plainText).split('\n').size();
}

bool MessageLabel::isMaximumTextLength(const QString & text)
{
	return MessageLabel::numberOfRows(text) >= MAX_ROWS_NUMBER;
}

std::vector<QString> MessageLabel::getImagesFromHtml(const QString & html)
{
	std::vector<QString> images;
	int firstIndx, lastIndx;

	firstIndx = html.indexOf(QRegExp("<img src"), 0);
	while (firstIndx != -1)
	{
		lastIndx = html.indexOf(QRegExp(">"), firstIndx);
		images.push_back(html.mid(firstIndx, lastIndx - firstIndx + 1));
		firstIndx = html.indexOf(QRegExp("<img src"), lastIndx + 1);
	}

	return images;
}

void MessageLabel::keyPressEvent(QKeyEvent * event)
{
	if ((event->key() == Qt::Key_C || event->key() == Qt::Key_Insert) && event->modifiers() == Qt::ControlModifier && this->hasSelectedText())
	{
		QApplication::clipboard()->setText(this->getSelectedText());
		return;
	}

	QLabel::keyPressEvent(event);
}