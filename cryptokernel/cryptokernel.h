// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This is the kernel of this program. No frameworks here! Only STL.
// crypto_kernel class uses random_map and generator::dev_random<T>
// for managing types and their fields.

// NOTE: type and field ids are NOT const, sometimes cryptokernel
// changes them: on every program start or more often.

#ifndef CRYPTOKERNEL_H
#define CRYPTOKERNEL_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "types.h"
#include "random_map.h"
#include "generator/dev_random.h"

class crypto_kernel
{
public:
  // Static functions
	
  // End of static functions
	
	crypto_kernel();
	~crypto_kernel();
	
  // Input/output functions
	// Reads all data from given stream. If any error occured, don't erase old data
	int read(std::istream &s);	// NOT IMPLEMENTED YET
	
	// Writes all data to given stream
	int write(std::ostream &s) const;	// NOT IMPLEMENTED YET
  // End of input/output functions
	
  // Type management (every type has own set of fields)
	// Returns all types ids
	std::vector<type_id_t> types() const;
	
	// Returns type id if it exists or empty string
	std::string type(type_id_t tid) const;
	
	// Generates new type id and adds it with name
	type_id_t add_type(const std::string &type_name);
	
	// Sets new name for existing type or returns invalid_type_id
	type_id_t set_type(type_id_t tid, const std::string &type_name);
  // End of type management
	
	
  // Field management (field ids are unique for same type)
	// Returns all fields ids for type or empty vector
	std::vector<field_id_t> fields(type_id_t tid) const;
	
	// Returns field id if it exists or empty string
	std::string field(type_id_t tid, field_id_t fid) const;
	
	// Generates new field id and adds it with name
	field_id_t add_field(type_id_t tid, const std::string &field_name);
	
	// Sets new name for existing field of existing type or returns invalid_field_id
	field_id_t set_field(type_id_t tid, field_id_t fid, const std::string &field_name);
  // End of field management
private:
	struct type
	{
		std::string name;
		random_map<field_id_t, std::string, generator::dev_random<field_id_t>> fields;
	};
	
	// Types map
	random_map<type_id_t, type, generator::dev_random<type_id_t>> types_;
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the kernel!")]]
	crypto_kernel(const crypto_kernel &other);
	
	[[deprecated("Don't copy the kernel!")]]
	crypto_kernel & operator=(const crypto_kernel &other);
};

#endif // CRYPTOKERNEL_H
