#pragma once
#include "ChatSession.h"

#include <QDialog>
#include "ui_NewUserDialog.h"

#include <vector>
#include <string>


class NewUserDialog : public QDialog
{
	Q_OBJECT

private:
	ChatSession & chatSession;
	std::string photoSource;

public:
	NewUserDialog(ChatSession & _chatSession, QWidget * parent);

	void connectSignalsAndSlots();

	std::vector<std::string> newUserValues();
	bool areAllFieldsCompleted();
	void validateName();
	void validateUserName();
	void choosePhoto();

	void NextButtonBehaviour();

public slots:
	void reject() override;

private:
	Ui::NewUserDialog ui;
};
