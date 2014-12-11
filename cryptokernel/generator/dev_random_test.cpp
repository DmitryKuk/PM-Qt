#include <iostream>

#include "dev_random.h"

int main()
{
	generator::universal_dev_random g;
	for (int i = 0; i < 150000; ++i) {
		int x;
		double y;
		unsigned char z;
		g >> x >> y >> z;
		std::cout << x << '\t' << y << '\t' << (int)z << std::endl;
	}
	return 0;
}
