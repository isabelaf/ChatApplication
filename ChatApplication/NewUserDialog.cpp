#include "NewUserDialog.h"
#include "Appearance.h"
#include "UserValidator.h"
#include "Server.h"
#include "Util.h"
#include "Administrator.h"

#include <qfiledialog.h>
#include <qmessagebox.h>


NewUserDialog::NewUserDialog(ChatSession & _chatSession, QWidget * parent)
	: chatSession{ _chatSession }, QDialog(parent)
{
	ui.setupUi(this);
	Appearance::setMainWidgetAppearance(this, DIALOG_FONT_SIZE);

	this->ui.UserNameValue->setMaxLength(15);
	this->ui.NameValue->setMaxLength(15);

	this->connectSignalsAndSlots();
}

void NewUserDialog::connectSignalsAndSlots()
{
	connect(this->ui.NextButton, &QPushButton::clicked, this, [this]() { this->ui.Step1->setEnabled(false); this->ui.Step2->setEnabled(true); });
	connect(this->ui.UploadPhotoButton, &QPushButton::clicked, this, &NewUserDialog::choosePhoto);
	connect(this->ui.ConnectButton, &QPushButton::clicked, this, &NewUserDialog::accept);

	connect(this->ui.UserNameValue, &QLineEdit::textChanged, this, &NewUserDialog::validateUserName);
	connect(this->ui.NameValue, &QLineEdit::textChanged, this, &NewUserDialog::validateName);

	connect(this->ui.UserNameValue, &QLineEdit::textChanged, this, &NewUserDialog::NextButtonBehaviour);
	connect(this->ui.NameValue, &QLineEdit::textChanged, this, &NewUserDialog::NextButtonBehaviour);
}

std::vector<std::string> NewUserDialog::newUserValues()
{
	std::vector<std::string> values;

	values.push_back(this->ui.UserNameValue->text().toStdString());
	values.push_back(this->ui.NameValue->text().toStdString());
	values.push_back(this->photoSource);

	return values;
}

bool NewUserDialog::areAllFieldsCompleted()
{
	if (UTIL::removeWhitespaceCharacters(this->ui.UserNameValue->text().toStdString()).size() == 0 || this->ui.UserNameErrors->text().size() != 0)
		return false;

	if (UTIL::removeWhitespaceCharacters(this->ui.NameValue->text().toStdString()).size() == 0 || this->ui.NameErrors->text().size() != 0)
		return false;

	return true;
}

void NewUserDialog::validateName()
{
	if (this->ui.NameValue->text().size() == 0)
	{
		this->ui.NameErrors->clear();
		return;
	}
	
	std::string errors = UserValidator::validateName(this->ui.NameValue->text().toStdString());

	if (errors.size() != 0)
		this->ui.NameErrors->setText(QString::fromStdString(errors));
	else
		this->ui.NameErrors->clear();
}

void NewUserDialog::validateUserName()
{
	if (this->ui.UserNameValue->text().size() == 0)
	{
		this->ui.UserNameErrors->clear();
		return;
	}

	std::string errors = UserValidator::validateUserName(this->ui.UserNameValue->text().toStdString(), this->chatSession);

	if (errors.size() != 0)
	{
		this->ui.UserNameErrors->setText(QString::fromStdString(errors));
	}
	else
	{
		this->ui.UserNameErrors->clear();
	}
}

void NewUserDialog::choosePhoto()
{
	QFileDialog dialog(this, QString(), "User\\Pictures\\Camera Roll");
	dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

	if (dialog.exec())
	{
		auto photo = dialog.selectedFiles();

		if (photo.size() == 0)
			return;

		this->photoSource = SERVER::uploadUserPhotoToServer(
			photo[0].toStdString(), 
			this->ui.UserNameValue->text().toStdString(),
			true);

		if (this->photoSource.size() == 0)
		{
			QMessageBox::warning(this, "Chat Application", "Error! Cannot upload photo!");

			this->ui.PhotoName->setToolTip("");
			this->ui.PhotoName->setText("No photo chosen.");
			this->ui.ConnectButton->setEnabled(false);
		}
		else
		{
			this->ui.PhotoName->setToolTip(photo[0]);
			this->ui.PhotoName->setText(photo[0].split('/').back());
			this->ui.ConnectButton->setEnabled(true);
		}
	}
}

void NewUserDialog::NextButtonBehaviour()
{
	if (this->areAllFieldsCompleted())
	{
		this->ui.NextButton->setEnabled(true);
	}
	else
	{
		this->ui.NextButton->setEnabled(false);
	}
}

void NewUserDialog::reject()
{
	if (this->photoSource.size() != 0)
	{
		SERVER::deleteFileFromServer(this->photoSource);
	}

	QDialog::reject();
}