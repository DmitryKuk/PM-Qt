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


int main()
{
	std::vector<type_id_t> types;
	cryptokernel k;
	
	k.add_type("hello");
	k.add_type("world");
	types = k.types();
	
	k.add_field(types[0], "hello field 1");
	k.add_field(types[0], "hello field 2");
	
	k.add_field(types[1], "world field 1");
	k.add_field(types[1], "world field 2");
	k.add_field(types[1], "world field 3");
	
	std::cout << types << std::endl;
	
	std::cout << "Types:" << std::endl;
	for (auto &t: types) {
		std::cout << IDENT << t << ":  " << k.type(t);
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
