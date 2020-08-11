#include "Administrator.h"
#include "Appearance.h"
#include "Server.h"
#include "PasswordLineEdit.h"

#include <qhboxlayout>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qlabel.h>

#include <algorithm>
#include <fstream>


Administrator::Administrator(ChatSession & _chatSession, QWidget * parent) : 
	chatSession{ _chatSession }, QWidget(parent)
{
	ui.setupUi(this);
	Appearance::setMainWidgetAppearance(this);

	this->connectSignalsAndSlots();
}

void Administrator::connectSignalsAndSlots()
{
	connect(this->ui.EnableUsersButton, &QPushButton::clicked, this, &Administrator::enableUsers);
	connect(this->ui.DisableUsersButton, &QPushButton::clicked, this, &Administrator::disableUsers);
	connect(this->ui.DisconnectUsersButton, &QPushButton::clicked, this, &Administrator::disconnectUsers);
	connect(this->ui.AccessServerButton, &QPushButton::clicked, this, &Administrator::accessServer);
	connect(this->ui.OpenErrorsFileButton, &QPushButton::clicked, this, &Administrator::openErrorsFile);
	connect(this->ui.ClearErrorsFileButton, &QPushButton::clicked, this, &Administrator::clearErrors);
}

void Administrator::disableUsers()
{
	std::vector<User> users = this->chatSession.getUsers();
	std::vector<User> enabledUsers;
	std::copy_if(
		users.begin(), 
		users.end(), 
		std::back_inserter(enabledUsers), 
		[](const User & u) { return u.isEnabled(); });

	std::vector<User> toDisable = this->chosenUsers(enabledUsers);

	for (auto u: toDisable)
		this->chatSession.disableUser(u);
}

void Administrator::enableUsers()
{
	std::vector<User> users = this->chatSession.getUsers();
	std::vector<User> disabledUsers;
	std::copy_if(
		users.begin(),
		users.end(),
		std::back_inserter(disabledUsers),
		[](const User & u) { return !u.isEnabled(); });

	std::vector<User> toEnable = this->chosenUsers(disabledUsers);
	
	for (auto u: toEnable)
		this->chatSession.enableUser(u);
}

void Administrator::disconnectUsers()
{
	std::vector<User> users = this->chatSession.getUsers();
	std::vector<User> toDisconect = chosenUsers(users);

	for (auto u : toDisconect)
		this->chatSession.disconnectUser(u);
}

std::vector<User> Administrator::chosenUsers(const std::vector<User> & userList)
{
	QDialog dialog{ this };
	dialog.setWindowTitle("Chat Application");

	QVBoxLayout * layout = new QVBoxLayout{ &dialog };

	QLabel * Instruction = new QLabel{ "Choose users:" };
	
	QPushButton * OKButton = new QPushButton{ "OK" };
	OKButton->setEnabled(false);

	QPushButton * SelectAllButton = new QPushButton{ "Select All" };
	if (userList.size() == 0) SelectAllButton->setEnabled(false);

	QListWidget * UserListWidget = new QListWidget{};

	if (userList.size() == 0)
	{
		UserListWidget->setSelectionMode(QAbstractItemView::NoSelection);

		QListWidgetItem * emptyListItem = new QListWidgetItem{ "No users." };
		emptyListItem->setFont(QFont{ "", -1, -1, true });

		UserListWidget->addItem(emptyListItem);
	}
	else
	{
		UserListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

		for (auto user : userList)
		{
			QListWidgetItem * userItem = new QListWidgetItem{ QString::fromStdString(user.getUserName()) };
			UserListWidget->addItem(userItem);
		}
	}

	layout->addWidget(Instruction);
	layout->addWidget(UserListWidget);
	layout->addWidget(OKButton);
	layout->addWidget(SelectAllButton);

	Appearance::setMainWidgetAppearance(&dialog, DIALOG_FONT_SIZE);

	connect(OKButton, &QPushButton::clicked, this, [&dialog]() { dialog.accept(); });
	connect(SelectAllButton, &QPushButton::clicked, this, [UserListWidget]() { UserListWidget->selectAll(); });
	connect(UserListWidget, &QListWidget::itemSelectionChanged, this, [OKButton]() { OKButton->setEnabled(true); });

	if (dialog.exec())
	{
		if (QMessageBox(QMessageBox::Question, "Chat Application", "Are you sure you want to perform this operation?", QFlags<QMessageBox::StandardButton>{ QMessageBox::Yes, QMessageBox::Abort }).exec() == QMessageBox::Yes)
		{
			auto selectedUsers = UserListWidget->selectedItems();
			std::vector<User> chosen;

			std::transform(
				selectedUsers.begin(),
				selectedUsers.end(),
				std::back_inserter(chosen),
				[UserListWidget, userList](QListWidgetItem * userItem) { return userList[UserListWidget->row(userItem)]; });

			return chosen;
		}
		return std::vector<User>();
	}
	else
		return std::vector<User>();
}

void Administrator::accessServer()
{
	if (SERVER::accessServer() == false)
		QMessageBox::warning(this, "Chat Application", "Could not access server.");
}

void Administrator::openErrorsFile()
{
	if (SERVER::openErrorsFile() == false)
		QMessageBox::warning(this, "Chat Application", "Could not open errors file.");
}

void Administrator::clearErrors()
{
	SERVER::clearErrors();
	this->openErrorsFile();
}

bool Administrator::askForAdminPassword()
{ 
	std::ifstream f(ADMINPASS_FILE_LOC);
	std::string password;
	f >> password;

	if (password.size() == 0)
		return false;

	QDialog dialog{ this };
	dialog.setWindowTitle("Chat Application");
	dialog.setMinimumWidth(270);

	QVBoxLayout * layout = new QVBoxLayout{ &dialog };

	QLabel * PasswordField = new QLabel{ "Enter password:" };

	PasswordLineEdit * PasswordValue = new PasswordLineEdit{};

	QPushButton * OKButton = new QPushButton{ "OK" };
	OKButton->setEnabled(false);

	layout->addWidget(PasswordField);
	layout->addWidget(PasswordValue);
	layout->addWidget(OKButton);

	Appearance::setMainWidgetAppearance(&dialog, DIALOG_FONT_SIZE);

	connect(OKButton, &QPushButton::clicked, this, [this, &dialog, PasswordValue, password]() { 
		
		if (password == PasswordValue->text().toStdString())
		{
			dialog.accept();
		}
		else
		{
			QMessageBox::warning(this, "Chat Application", "Incorrect password!");
			PasswordValue->clear();
		}
	});

	connect(PasswordValue, &QLineEdit::textEdited, this, [OKButton](const QString & text) { 
		if (text.size() != 0)
			OKButton->setEnabled(true);
		else OKButton->setEnabled(false);
	});

	if (dialog.exec())
	{
		return true;
	}
	return false;
}

void Administrator::setVisible(bool visible)
{
	if (visible == true)
	{
		if (this->askForAdminPassword())
		{
			QWidget::setVisible(true);
		}
		else
		{
			QMessageBox::warning(this, "Chat Application", "Can't log in as administrator.");
			QWidget::setVisible(false);
		}
	}
	else
	{
		QWidget::setVisible(false);
	}
}