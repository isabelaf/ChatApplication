#pragma once

#include <QLabel>

#include <vector>


class MessageLabel : public QLabel
{
	Q_OBJECT

private:
	std::vector<int> wordWrapChars;

public:
	MessageLabel(const QString & text = QString(), QWidget * parent = Q_NULLPTR);
	
	QString getSelectedText();

	void setWrappedText(const QString & text);

	static QString wrapText(const QString & text, std::vector<int> & wordWrapChars = std::vector<int>());
	static QString wrapHtmlText(const QString & html, std::vector<int> & wordWrapChars = std::vector<int>());
	static QString wrapPlainText(const QString & plainText, std::vector<int> & wordWrapChars = std::vector<int>());
	static QString removeWrapChars(const QString & text, const std::vector<int> & wordWrapChars);
	static QString removeExtraText(const QString & text);

	static int numberOfRows(const QString & text);
	static int numberOfRowsHtmlText(const QString & html);
	static int numberOfRowsPlainText(const QString & plainText);

	static bool isMaximumTextLength(const QString & text);

private:
	static std::vector<QString> getImagesFromHtml(const QString & html);

protected:
	void keyPressEvent(QKeyEvent * event) override;
};
