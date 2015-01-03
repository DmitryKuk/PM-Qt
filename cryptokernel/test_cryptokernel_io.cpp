// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <iostream>
#include <sstream>

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
	cryptokernel k1;
	
	// Adding types
	auto t1 = k1.add_type("type_1");
	auto t1f1 = k1.add_type_field(t1, "type_1_field_1", "*");
	auto t1f2 = k1.add_type_field(t1, "type_1_field_2", "*");
	
	auto t2 = k1.add_type("type_2");
	auto t2f1 = k1.add_type_field(t2, "type_2_field_1", "*");
	
	// Adding data
	auto rg = k1.add_root_group();
	auto g1 = k1.add_group(rg, "group_1");
	auto g1r1 = k1.add_record(g1, "record_1", t1);
	auto g1r1f1 = k1.add_field(g1r1, t1f1, "record_1_field_1");
	
	if (g1r1f1 == invalid_rfield_id) std::cerr << "ERROR: Field not inserted!" << std::endl;
	
	std::stringstream s1, s2;
	if (!k1.write(s1)) std::cout << "Types not wrote by k1!" << std::endl;
	std::cout << std::endl;
	
	cryptokernel k2;
	if (!k2.read(s1)) std::cout << "Types not read by k1!" << std::endl;
	
	if (!k2.write(s2)) std::cout << "Types not wrote by k1!" << std::endl;
	std::cout << std::endl;
	
	if (s1.str() == s2.str())
		std::cout << "OK: Outputs are the same." << std::endl;
	else
		std::cout << "S1:" << std::endl << s1.str() << std::endl
				  << "S2:" << std::endl << s2.str() << std::endl;
	
	// Printing result
	// std::cerr << std::endl << "Types:" << std::endl;
	// for (auto &t: k1.types()) {
	// 	std::cerr << IDENT << t << ":  " << ((k1.test_type(t))? k1.type_name(t): "<NOT EXISTS>");
	// 	auto type_fields = k1.type_fields(t);
	// 	if (type_fields.empty())
	// 		std::cerr << std::endl;
	// 	else {
	// 		std::cerr << "  (" << type_fields.size() << "):" << std::endl;
	// 		for (auto &f: type_fields)
	// 			std::cerr << IDENT IDENT << f << ":  " << k1.type_field_name(t, f) << std::endl;
	// 	}
	// }
	return 0;
}
