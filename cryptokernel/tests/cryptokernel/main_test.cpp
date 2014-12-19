// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <iostream>
#include <vector>
#include <list>

#include "project/types.h"
#include "project/cryptokernel.h"

#define IDENT "    "


template<class T>
std::ostream & operator<<(std::ostream &f, const std::vector<T> &v)
{
	f << "Vector size: " << v.size() << ':' << std::endl;
	size_t i = 0;
	for (const auto &x: v)
		f << IDENT "[" << i++ << "]  " << x << std::endl;
	return f;
}


template<class T>
std::ostream & operator<<(std::ostream &f, const std::pair<T, T> &p)
{
	return f << '(' << p.first << ", " << p.second << ')';
}

int main()
{
	std::vector<type_id_t> types;
	cryptokernel k;
	
	// Adding types
	k.add_type("hello");
	k.add_type("world");
	types = k.types();
	std::cout << "Types: " << types << std::endl;
	
	// Addidng fields
	k.add_field(types[0], "hello field 1");
	k.add_field(types[0], "hello field 2");
	
	k.add_field(types[1], "world field 1");
	k.add_field(types[1], "world field 2");
	k.add_field(types[1], "world field 3");
	
	// Type changing
	std::cout << ((k.set_type(types[0], "new hello") != invalid_type_id)?
				 "OK: Correct type changed":
				 "ERROR: Can't set correct type") << std::endl;
	
	std::cout << ((k.set_type(0, "new incorrect type") == invalid_type_id)?
				 "OK: Incorrect type not changed":
				 "ERROR: Invalid type changed") << std::endl;
	
	// Field changing
	std::cout << ((k.set_field(types[0], k.fields(types[0])[0], "hello new field 1") != invalid_type_id)?
				 "OK: Correct type, correct field changed":
				 "ERROR: Can't set correct type, correct field") << std::endl;
	
	std::cout << ((k.set_field(types[0], 0, "hello new incorrect field") == invalid_field_id)?
				 "OK: Correct type, incorrect field not changed":
				 "ERROR: Changed correct type, incorrect field") << std::endl;
	
	std::cout << ((k.set_field(0, 0, "incorrect type new incorrect field") == invalid_field_id)?
				 "OK: Incorrect type, incorrect field not changed":
				 "ERROR: Changed incorrect type, incorrect field") << std::endl;
	
	// Removing types
	std::cout << ((k.remove_type(types[1]) != invalid_type_id)?
				 "OK: Correct type removed":
				 "ERROR: Correct type not removed") << std::endl;
	
	std::cout << ((k.remove_type(0) == invalid_type_id)?
				 "OK: Incorrect type not removed":
				 "ERROR: Incorrect type removed") << std::endl;
	
	// Printing result
	std::cout << std::endl << "Types:" << std::endl;
	for (auto &t: k.types()) {
		std::cout << IDENT << t << ":  " << ((k.test_type(t))? k.type(t): "<NOT EXISTS>");
		auto fields = k.fields(t);
		if (fields.empty())
			std::cout << std::endl;
		else {
			std::cout << "  (" << fields.size() << "):" << std::endl;
			for (auto &f: fields)
				std::cout << IDENT IDENT << f << ":  " << k.field(t, f) << std::endl;
		}
	}
	
	return 0;
}
