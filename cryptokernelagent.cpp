// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <iostream>
#include <fstream>

CryptoKernelAgent::CryptoKernelAgent()
{
	std::ifstream f("test_input.txt");
	if (!kernel_.read(f))
		std::cerr << "ERROR: Cryptokernel agent: kernel can't read from test_input.txt!" << std::endl;
}

CryptoKernelAgent::~CryptoKernelAgent()
{}
