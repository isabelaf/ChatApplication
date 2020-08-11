#include "ChatApplication.h"
#include "ChatWindow.h"
#include "Administrator.h"

#include <QtWidgets/QApplication>
#include <qdesktopwidget.h>


int main(int argc, char * argv[])
{
	QApplication a(argc, argv);

	ChatSession chatSession{};

	ChatApplication chatApp{ chatSession };
	chatApp.show();

	Administrator admin{ chatSession };
	admin.show();

	ChatWindow * w1  = chatApp.connectToChat(std::vector<std::string>{ 
		"isabela", 
		"Isabela", 
		"User Photos/isabela.jpg" 
	});

	ChatWindow * w2 = chatApp.connectToChat(std::vector<std::string>{
		"daniel", 
		"Daniel", 
		"User Photos/daniel.jpeg" 
	});

	ChatWindow * w3 = chatApp.connectToChat(std::vector<std::string>{
		"ezthecat", 
		"EZ", 
		"User Photos/ezthecat.jpg" 
	});

	//
	QRect screenRect = QApplication::desktop()->screen()->rect();
	int x = screenRect.x(), y = screenRect.y(), width = screenRect.width(), height = screenRect.height();

	chatApp.move(x + 25, y + 25);
	w1->move(x + width - w1->width() - 25, y + 25);
	w2->move(x + 25, y + height - w2->height() - 100);
	w3->move(x + width - w3->width() - 25, y + height - w2->height() - 100);
	admin.move(x + width / 2 - admin.width() / 2, y + height / 2 - admin.height() / 2);
	//

	return a.exec();
}