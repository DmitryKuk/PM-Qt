#include <iostream>

#include "generator/dev_random.h"
#include "random_map.h"

#define PRINT(m) do { for (const auto &x: m) std::cout << '\t' << x.first << '\t' << x.second << std::endl; } while (false)
#define INSERT(m, val) do { auto p = m.insert(val); std::cout << (p.second? "Inserted: ": "Not inserted: ") << val << std::endl; } while (false)

int main()
{
	// std::cin.get();
	random_map<unsigned short, unsigned short, generator::dev_random<unsigned short>> m;
	std::cout << (m.generator().status() == generator::status::not_initialized? "Generator is not initialized!": "Generator is ok.") << std::endl;
	
	for (unsigned short i = 0; i < 1000; ++i)
		INSERT(m, i);
	// INSERT(m, 10);
	// INSERT(m, 15);
	// INSERT(m, 20);
	PRINT(m);
	
	std::cout << m.size() << std::endl;
	// std::cin.get();
	return 0;
}
