// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <iostream>
#include <fstream>


CryptoKernelAgent::CryptoKernelAgent():
	mainWindow_(new MainWindow(this)),
	kernel_(new cryptokernel)
{
	std::ifstream f("test_input.txt");
	if (!this->kernel_->read(f))
		std::cerr << "ERROR: CryptoKernelAgent constructor: Kernel can't read data!" << std::endl;
}

CryptoKernelAgent::~CryptoKernelAgent()
{
	std::ofstream f("test_input.txt");
	if (!this->kernel_->write(f))
		std::cerr << "ERROR: CryptoKernelAgent destructor: Kernel can't write data!" << std::endl;
}


void CryptoKernelAgent::run()
{
	this->DATA_loadData();
	this->GUI_mainWindow()->show();
	this->GUI_mainWindow()->mainWidget()->recordContentWidget()->hide();
}
