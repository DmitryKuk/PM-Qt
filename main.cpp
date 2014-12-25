// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	MainWindow w;
	w.show();
	return a.exec();
}
