// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <QApplication>

#include "mainwindow.h"
#include "cryptokernelagent.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	CryptoKernelAgent agent;
	
	MainWindow w;
	w.show();
	return a.exec();
}
