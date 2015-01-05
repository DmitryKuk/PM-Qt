// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <QApplication>

#include "cryptokernelagent.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	CryptoKernelAgent agent;
	agent.run();
	
	return app.exec();
}
