// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <iostream>
#include <vector>
#include <list>

#include "types.h"
#include "cryptokernel.h"

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
	return f << '(' << p.first << ", " << p.second.format << ')';
}


int main()
{
	std::vector<type_id_t> types;
	cryptokernel k;
	
	// Adding types
	k.add_type("hello");
	k.add_type("world");
	types = k.types();
	std::cerr << "Types: " << types << std::endl;
	
	// Addidng type_fields
	k.add_type_field(types[0], "hello field 1");
	k.add_type_field(types[0], "hello field 2");
	
	k.add_type_field(types[1], "world field 1");
	k.add_type_field(types[1], "world field 2");
	k.add_type_field(types[1], "world field 3");
	
	// Type changing
	
	std::cerr << "Setting \"new hello\"..." << std::endl
			  << ((k.set_type_name(types[0], "new hello") != invalid_type_id)?
				 "OK: Correct type changed":
				 "ERROR: Can't set correct type") << std::endl;
	
	std::cerr << "Setting \"new hello 1\"..." << std::endl
			  << ((k.set_type_name(types[0], "new hello 1") != invalid_type_id)?
				 "OK: Correct type changed":
				 "ERROR: Can't set correct type") << std::endl;
	
	std::cerr << "Setting \"new hello\" again..." << std::endl
			  << ((k.set_type_name(types[0], "new hello") != invalid_type_id)?
				 "OK: Correct type changed":
				 "ERROR: Can't set correct type") << std::endl;
	
	
	std::cerr << ((k.set_type_name(0, "new incorrect type") == invalid_type_id)?
				 "OK: Incorrect type not changed":
				 "ERROR: Invalid type changed") << std::endl;
	
	// Field changing
	std::cerr << ((k.set_type_field_name(types[0], k.type_fields(types[0])[0], "hello new field 1") != invalid_type_id)?
				 "OK: Correct type, correct field changed":
				 "ERROR: Can't set correct type, correct field") << std::endl;
	
	std::cerr << ((k.set_type_field_name(types[0], 0, "hello new incorrect field") == invalid_tfield_id)?
				 "OK: Correct type, incorrect field not changed":
				 "ERROR: Changed correct type, incorrect field") << std::endl;
	
	std::cerr << ((k.set_type_field_name(0, 0, "incorrect type new incorrect field") == invalid_tfield_id)?
				 "OK: Incorrect type, incorrect field not changed":
				 "ERROR: Changed incorrect type, incorrect field") << std::endl;
	
	// Removing types
	std::cerr << ((k.remove_type(types[1]) != invalid_type_id)?
				 "OK: Correct type removed":
				 "ERROR: Correct type not removed") << std::endl;
	
	std::cerr << ((k.remove_type(0) == invalid_type_id)?
				 "OK: Incorrect type not removed":
				 "ERROR: Incorrect type removed") << std::endl;
	
	// Printing result
	std::cerr << std::endl << "Types:" << std::endl;
	for (auto &t: k.types()) {
		std::cerr << IDENT << t << ":  " << ((k.test_type(t))? k.type_name(t): "<NOT EXISTS>");
		auto type_fields = k.type_fields(t);
		if (type_fields.empty())
			std::cerr << std::endl;
		else {
			std::cerr << "  (" << type_fields.size() << "):" << std::endl;
			for (auto &f: type_fields)
				std::cerr << IDENT IDENT << f << ":  " << k.type_field_name(t, f) << std::endl;
		}
	}
	
	return 0;
}
