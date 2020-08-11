#pragma once
#include "ChatSession.h"

#include <QWidget>
#include "ui_Administrator.h"

#include <qdialog.h>

#include <vector>


class Administrator : public QWidget
{
	Q_OBJECT

private:
	ChatSession & chatSession;

public:
	Administrator(ChatSession & _chatSession, QWidget * parent = Q_NULLPTR);

	void connectSignalsAndSlots();

	void disableUsers();
	void enableUsers();
	void disconnectUsers();

	std::vector<User> chosenUsers(const std::vector<User> & userList);

	void accessServer();
	void openErrorsFile();
	void clearErrors();

	bool askForAdminPassword();

	void setVisible(bool visible) override;

private:
	Ui::Administrator ui;
};
