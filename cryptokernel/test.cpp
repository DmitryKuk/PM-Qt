#include <iostream>

#include "interval_set.h"

#define PRINT(IS) for (auto &x: const_cast<const interval_set<int> &>(IS)) std::cout << "\t[" << x.first << ", " << x.second << ')' << std::endl

int main()
{
	interval_set<int> is;
	
	is.insert(1); PRINT(is); std::cout << std::endl;
	is.insert(3); PRINT(is); std::cout << std::endl;
	is.insert(2); PRINT(is); std::cout << std::endl;
	
	
	is.erase(2); PRINT(is); std::cout << std::endl;
	return 0;
}
