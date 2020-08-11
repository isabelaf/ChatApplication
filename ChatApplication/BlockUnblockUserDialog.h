#pragma once
#include "ChatSession.h"

#include <QDialog>
#include "ui_BlockUnblockUserDialog.h"


class BlockUnblockUserDialog : public QDialog
{
	Q_OBJECT

private:
	ChatSession & chatSession;
	bool block;

public:
	BlockUnblockUserDialog(ChatSession & _chatSession, const bool block, QWidget * parent = Q_NULLPTR);
	void init();
	void connectSignalsAndSlots();

	void ContinueButtonHandler();
	void BlockUnblockButtonHandler();

	std::pair<std::string, std::string> users();

private:
	Ui::BlockUnblockUserDialog ui;
};
