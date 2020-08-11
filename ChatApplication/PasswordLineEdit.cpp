#include "PasswordLineEdit.h"

#include <qevent.h>

#include <Windows.h>


PasswordLineEdit::PasswordLineEdit(QWidget * parent)
	: QLineEdit(parent)
{
	this->setEchoMode(QLineEdit::Password);
	this->setPlaceholderText("Enter password here...");
	
	this->capsOn = new QToolTip{};
}

void PasswordLineEdit::showCapsOnToolTip()
{
	QPoint pos = this->mapToGlobal(this->pos());
	pos.setX(pos.x() - 10);
	pos.setY(pos.y() - 33);

	this->capsOn->showText(pos, "Caps Lock is ON.");
}

void PasswordLineEdit::hideCapsOnToolTip()
{
	this->capsOn->hideText();
}

void PasswordLineEdit::keyPressEvent(QKeyEvent * event)
{
	if (GetKeyState(VK_CAPITAL) & 0x0001)
	{
		this->showCapsOnToolTip();
	}
	else
	{
		this->hideCapsOnToolTip();
	}

	QLineEdit::keyPressEvent(event);
}
