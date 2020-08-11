#include "MessageTextTextEdit.h"
#include "Appearance.h"
#include "EmojiView.h"
#include "Util.h"
#include "MessageLabel.h"

#include <qkeyevent>
#include <qscrollbar>
#include <qmimedata.h>
#include <qpainter>

#include <string>


MessageTextTextEdit::MessageTextTextEdit(QWidget * parent) : QTextEdit(parent)
{
	this->setPlaceholderText("Type your message...");

	connect(this, &MessageTextTextEdit::textChanged, this, &MessageTextTextEdit::textChangedEvents);
}

void MessageTextTextEdit::insertImage(const QString & imageSource, const int h, const int w)
{
	if (imageSource.size() == 0)
		return;

	QObject::blockSignals(true);
	this->insertHtml(
		"<img src='"
		+ imageSource
		+ "' height='"
		+ QString::number(h)
		+ "' width='"
		+ QString::number(w)
		+ "'>");
	QObject::blockSignals(false);
}

void MessageTextTextEdit::changeToEmoji()
{
	if (this->toPlainText().size() == 0)
		return;

	auto data = EmojiView::replaceEmojiCode(this->toHtml());

	if (std::get<1>(data) != -1)
	{
		QObject::blockSignals(true);

		this->setHtml(std::get<0>(data));

		// METHOD 1
		//QTextCursor cursor{ this->textCursor() };
		//cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, std::get<0>(data));
		//this->setTextCursor(cursor);

		// METHOD 2
		for (int i = 0; i < std::get<1>(data); i++)
			this->moveCursor(QTextCursor::Right);

		this->insertImage(std::get<2>(data), SIZE, SIZE);

		this->moveCursor(QTextCursor::Right);

		QObject::blockSignals(false);
	}
}

void MessageTextTextEdit::sendResizeSignals()
{
	if (this->verticalScrollBar()->minimum() == 0 && this->verticalScrollBar()->maximum() == 0)
	{
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::MoveOperation::End);
		int yCoord = this->cursorRect(cursor).y();

		if (this->isAtMaximumHeight == true && yCoord == this->document()->documentMargin())
		{
			this->isAtMaximumHeight = false;
			emit decreaseNeeded();
		}
	}
	else
	{
		if (this->isAtMaximumHeight == false)
		{
			this->isAtMaximumHeight = true;
			emit expansionNeeded();
		}
	}
}

void MessageTextTextEdit::textChangedEvents()
{
	this->changeToEmoji();
	this->ensureCursorVisible();
	this->sendResizeSignals();

	if (MessageLabel::isMaximumTextLength(this->toHtml()))
	{
		this->setReadOnly(true);
		this->setTextInteractionFlags(this->textInteractionFlags() | Qt::TextSelectableByKeyboard);
	}
}

void MessageTextTextEdit::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
	auto modifier = event->modifiers();

	switch (key)
	{
	case Qt::Key_Return:
	{
		if (modifier == Qt::ShiftModifier)
		{
			this->insertPlainText("\n");
		}
		else
		{
			this->setReadOnly(false);
			emit enterPressed();
		}
		return;
	}
	case Qt::Key_Backspace:
		if (this->isReadOnly())
		{
			if (this->toolTip().size() != 0)
			{
				this->clear();
			}
			this->setReadOnly(false);
		} 
	}

	QTextEdit::keyPressEvent(event);
}

void MessageTextTextEdit::insertFromMimeData(const QMimeData * source)
{
	if (source->hasImage())
		return;

	if (source->hasText())
	{
		QString toInsert = MessageLabel::removeExtraText(source->text());

		QObject::blockSignals(true);
		this->insertHtml(EmojiView::replaceAllEmojiCodes(toInsert.toHtmlEscaped()));
		this->ensureCursorVisible();
		QObject::blockSignals(false);

		return;
	}

	QTextEdit::insertFromMimeData(source);
}
