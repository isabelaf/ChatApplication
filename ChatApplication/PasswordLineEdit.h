#pragma once

#include <QLineEdit>
#include <qtooltip.h>


class PasswordLineEdit : public QLineEdit
{
	Q_OBJECT

private:
	QToolTip * capsOn;

public:
	PasswordLineEdit(QWidget * parent = Q_NULLPTR);

	void showCapsOnToolTip();
	void hideCapsOnToolTip();

protected:
	void keyPressEvent(QKeyEvent * event) override;
};
