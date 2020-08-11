#include "Appearance.h"

#include <qabstractbutton.h>


QPalette Appearance::makePalette()
{
	QPalette palette{};

	palette.setColor(QPalette::Active, QPalette::WindowText, QColor(0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(0, 85, 0));

	palette.setColor(QPalette::Active, QPalette::Button, QColor(0, 170, 0));
	palette.setColor(QPalette::Inactive, QPalette::Button, QColor(0, 170, 0));
	palette.setColor(QPalette::Disabled, QPalette::Button, QColor(0, 170, 0));

	palette.setColor(QPalette::Active, QPalette::Light, QColor(0, 255, 0));
	palette.setColor(QPalette::Inactive, QPalette::Light, QColor(0, 255, 0));
	palette.setColor(QPalette::Disabled, QPalette::Light, QColor(0, 255, 0));

	palette.setColor(QPalette::Active, QPalette::Midlight, QColor(0, 212, 0));
	palette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(0, 212, 0));
	palette.setColor(QPalette::Disabled, QPalette::Midlight, QColor(0, 212, 0));

	palette.setColor(QPalette::Active, QPalette::Dark, QColor(0, 85, 0));
	palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(0, 85, 0));
	palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(0, 85, 0));

	palette.setColor(QPalette::Active, QPalette::Mid, QColor(0, 113, 0));
	palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(0, 113, 0));
	palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(0, 113, 0));

	palette.setColor(QPalette::Active, QPalette::Text, QColor(0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::Text, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0, 85, 0));

	palette.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
	palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
	palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));

	palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0, 85, 0));

	palette.setColor(QPalette::Active, QPalette::Base, QColor(255, 255, 255));
	palette.setColor(QPalette::Inactive, QPalette::Base, QColor(255, 255, 255));
	palette.setColor(QPalette::Disabled, QPalette::Base, QColor(0, 170, 0));

	palette.setColor(QPalette::Active, QPalette::Window, QColor(0, 170, 0));
	palette.setColor(QPalette::Inactive, QPalette::Window, QColor(0, 170, 0));
	palette.setColor(QPalette::Disabled, QPalette::Window, QColor(0, 170, 0));

	palette.setColor(QPalette::Active, QPalette::Shadow, QColor(0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(0, 0, 0));

	palette.setColor(QPalette::Active, QPalette::AlternateBase, QColor(127, 212, 127));
	palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QColor(127, 212, 127));
	palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(0, 170, 0));

	palette.setColor(QPalette::Active, QPalette::ToolTipBase, QColor(255, 255, 220));
	palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(255, 255, 220));
	palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor(255, 255, 220));

	palette.setColor(QPalette::Active, QPalette::ToolTipText, QColor(0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(0, 0, 0));
	palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor(0, 0, 0));

	return palette;
}

void Appearance::setChildrenPalette(const QObjectList & childrenList, const QPalette & palette)
{
	for (auto c : childrenList)
	{
		auto w = qobject_cast<QWidget*>(c);
		if (w != nullptr)
		{
			w->setPalette(palette);
		}
	}
}

void Appearance::setCursorForButtons(const QObjectList & childrenList, const QCursor & cursor)
{
	for (auto c : childrenList)
	{
		auto b = qobject_cast<QAbstractButton*>(c);
		if (b != nullptr)
		{
			b->setCursor(cursor);
		}
	}
}

QFont Appearance::makeFont(const int size)
{
	return QFont(FONT_FAMILY, size);
}

void Appearance::setMainWidgetAppearance(QWidget * widget, const int fontSize, const bool goForSubChildren)
{
	widget->setFont(makeFont(fontSize));
	Appearance::setChildrenPalette(widget->children());
	Appearance::setCursorForButtons(widget->children());

	if (goForSubChildren)
	{
		for (auto c : widget->children())
		{
			auto w = qobject_cast<QWidget*>(c);
			if (w != nullptr)
			{
				Appearance::setMainWidgetAppearance(w, fontSize, true);
			}
		}
	}
}