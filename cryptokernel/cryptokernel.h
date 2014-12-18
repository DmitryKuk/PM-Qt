// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This is the kernel of this program. No frameworks here! Only STL.
// cryptokernel class uses random_map and generator::dev_random<T>
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
#include "double_map.h"
#include "generator/dev_random.h"

class cryptokernel
{
public:
	// Constructors, destructor and operator=()
	cryptokernel();									// Default
	cryptokernel(cryptokernel &&other);				// Move
	~cryptokernel();
	
	cryptokernel & operator=(cryptokernel &&other);	// Move
	
  // Input/output functions
	// Reads all data from given stream. If any error occured, don't erase old data
	int read(std::istream &s);	// NOT IMPLEMENTED YET
	
	// Writes all data to given stream
	int write(std::ostream &s) const;	// NOT IMPLEMENTED YET
  // End of input/output functions
	
  // Type management (every type has own set of fields)
	// Returns all types ids (ids are ordered by user)
	std::vector<type_id_t> types() const;
	
	// Returns type id if it exists or empty string
	std::string type(type_id_t tid) const;
	
	// Generates new type id and adds it with name
	type_id_t add_type(const std::string &type_name);
	
	// Sets new name for existing type or returns invalid_type_id
	type_id_t set_type(type_id_t tid, const std::string &type_name);
  // End of type management
	
	
  // Field management (field ids are unique for same type)
	// Returns all fields ids for type (ids are ordered by user) or empty vector
	std::vector<field_id_t> fields(type_id_t tid) const;
	
	// Returns field id if it exists or empty string
	std::string field(type_id_t tid, field_id_t fid) const;
	
	// Generates new field id and adds it with name
	field_id_t add_field(type_id_t tid, const std::string &field_name);
	
	// Sets new name for existing field of existing type or returns invalid_field_id
	field_id_t set_field(type_id_t tid, field_id_t fid, const std::string &field_name);
  // End of field management
private:
  // Generators
	// Default generators types
	typedef generator::dev_random<type_id_t> type_generator_t;
	typedef generator::dev_random<field_id_t> field_genertor_t;
	
	type_generator_t type_generator_;	// Default type ids generator
	field_genertor_t field_generator_;	// Default type field ids generator
  // End of generators
	
  // Fields and types containers
	// Fields double map: field id, field name: format/description string
	typedef double_map<field_id_t, std::string, std::string> fields_dmap_t;
	
	// Types double map: type id, type name: field map
	typedef double_map<type_id_t, std::string, fields_dmap_t> types_dmap_t;
	
	types_dmap_t types_;	// Types map
  // End of fields and types containers
	
  // Fields and types orders containers
	typedef std::vector<field_id_t> fields_order_t;
	typedef std::vector<std::pair<type_id_t, fields_order_t>> types_order_t;
	
	types_order_t types_order_;
  // End of fields and types orders containers
	
	
  // Deprecated members
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy crypto_kernel!")]]
	cryptokernel(const cryptokernel &other);				// Copy
	
	[[deprecated("Don't copy crypto_kernel!")]]
	cryptokernel & operator=(const cryptokernel &other);	// Copy
  // End of deprecated members
};

#endif // CRYPTOKERNEL_H
