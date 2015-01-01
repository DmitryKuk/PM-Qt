#include <iostream>
#include <iomanip>
#include <unordered_map>

#include "generator/dev_random.h"
#include "random_map.h"

template<class T1, class T2>
inline
std::ostream & operator<<(std::ostream &f, const std::pair<T1, T2> p)
{
	return f << '(' << std::setw(6) << p.first
			 << ", " << std::setw(5) << p.second << ')';
}

template<class RM>
inline
void print(const RM &m)
{
	std::cout << "Random map contains " << m.size() << " elements:" << std::endl;
	for (const auto &x: m) std::cout << '\t' << x << std::endl;
}

template<class Generator, class RM, class Val>
inline
void insert_random(const Generator &g, RM &m, const Val &val)
{
	auto p = m.insert_random(g, val);
	if (p.second)
		std::cout << "\tInserted " << *p.first << ": OK" << std::endl;
	else
		std::cout << "\tNot inserted " << val << ": ERROR" << std::endl;
}

template<class RM, class Key, class Val>
void emplace(RM &m, const Key &key, const Val &val)
{
	auto p = m.emplace(key, val);
	if (p.second)
		std::cout << "\tEmplaced " << *p.first << ": OK" << std::endl;
	else
		std::cout << "\tNot emplaced " << std::make_pair(key, val) << ": ERROR" << std::endl;
};


int main()
{
	// std::cin.get();
	generator::dev_random<unsigned short> g;
	random_map<unsigned short, unsigned short, std::unordered_map> m;
	std::cout << ((g.status() == generator::status::not_initialized)?
				 "Generator is not initialized!":
				 "Generator is ok.") << std::endl;
	
	std::cout << "Emplacing fixed key-value pairs:" << std::endl;
	for (unsigned short i = 0; i < 100; ++i)
		emplace(m, i, i * 100);
	
	std::cout << "Emplacing random values:" << std::endl;
	for (unsigned short i = 0; i < 100; ++i)
		insert_random(g, m, i);
	
	std::cout << "Trying to emplace values with existing keys:" << std::endl;
	for (auto &x: m) {
		auto p = m.emplace(x.first, x.second);
		std::cout << "\tTry " << x << ": " << (p.second? "inserted: ERROR": "not inserted: OK") << std::endl;
	}
	
	print(m);
	// std::cin.get();
	return 0;
}
