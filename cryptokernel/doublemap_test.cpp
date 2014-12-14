// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <iostream>
#include <string>

#include "doublemap.h"

int main()
{
	doublemap<int, std::string, int> m;
	
	std::cout << "Inserting:" << std::endl;
	m.insert(make_triple(1, std::string("one"), 1));
	m.insert(make_triple(2, std::string("two"), 2));
	
	for (const auto &x: m)
		std::cout << '\t' << x.first << ", " << x.second << ":  " << x.third << std::endl;
	std::cout << std::endl;
	
	std::cout << "Erasing:" << std::endl;
	m.erase1(1);
	m.erase1(3);
	for (const auto &x: m)
		std::cout << '\t' << x.first << ", " << x.second << ":  " << x.third << std::endl;
	std::cout << std::endl;
	
	std::cout << "Searching:" << std::endl;
	auto it = m.find2("two");
	std::cout << "\ttwo:  ";
	if (it == m.end()) std::cout << "Not found." << std::endl;
	else std::cout << it->first << ", " << it->second << ":  " << it->third << std::endl;
	
	it = m.find2("three");
	std::cout << "\tthree:  ";
	if (it == m.end()) std::cout << "Not found." << std::endl;
	else std::cout << it->first << ", " << it->second << ":  " << it->third << std::endl;
	
	it = m.find1(2);
	std::cout << "\t2:  ";
	if (it == m.end()) std::cout << "Not found." << std::endl;
	else std::cout << it->first << ", " << it->second << ":  " << it->third << std::endl;
	
	it = m.find1(5);
	std::cout << "\t5:  ";
	if (it == m.end()) std::cout << "Not found." << std::endl;
	else std::cout << it->first << ", " << it->second << ":  " << it->third << std::endl;
	
	return 0;
}
