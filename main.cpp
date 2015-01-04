// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <QApplication>

#include "mainwindow.h"
#include "cryptokernelagent.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	MainWindow mainWindow;
	CryptoKernelAgent agent;
	
	connect(&agent, &mainWindow);	// Connect agent and main window
	agent.showData();				// Agent sends data into interface
	
	mainWindow.show();
	return a.exec();
}
