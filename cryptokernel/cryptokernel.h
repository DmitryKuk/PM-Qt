// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This is the kernel of this program. No frameworks here! Only STL.

#ifndef CRYPTOKERNEL_H
#define CRYPTOKERNEL_H

#include <vector>
#include <string>

#include "types.h"

class crypto_kernel
{
public:
	crypto_kernel();
	~crypto_kernel();
	
	
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
	[[deprecated("Don't copy the kernel!")]]
	crypto_kernel(const crypto_kernel &other);
	
	[[deprecated("Don't copy the kernel!")]]
	crypto_kernel & operator=(const crypto_kernel &other);
};

#endif // CRYPTOKERNEL_H
