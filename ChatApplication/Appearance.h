#pragma once

#include <qobject.h>
#include <qcursor.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qfont.h>
#include <qwidget.h>


#define SIZE 20
#define FONT_FAMILY "Segoe UI"
#define FONT_SIZE 12
#define DIALOG_FONT_SIZE 10

#define MAX_TEXT_WIDTH 380
#define MAX_ROWS_NUMBER 8


namespace Appearance
{
	QPalette makePalette();

	void setChildrenPalette(const QObjectList & childrenList, const QPalette & palette = makePalette());

	void setCursorForButtons(const QObjectList & childrenList, const QCursor & cursor = QCursor(Qt::PointingHandCursor));

	QFont makeFont(const int size = FONT_SIZE);

	void setMainWidgetAppearance(QWidget * widget, const int fontSize = FONT_SIZE, const bool goForSubChildren = true);
}