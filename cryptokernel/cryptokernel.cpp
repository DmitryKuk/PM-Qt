// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernel.h"

crypto_kernel::crypto_kernel()
{}

crypto_kernel::~crypto_kernel()
{}


// Input/output functions
// Reads all data from given stream. If any error occured, don't erase old data
int
CryptoKernel::read(std::istream &s)
{
	std::cerr << "cryptokernel::read not implemented yet!" << std::endl;
}

// Writes all data to given stream
int
CryptoKernel::write(std::ostream &s) const
{
	std::cerr << "cryptokernel::read not implemented yet!" << std::endl;
}
// End of input/output functions


// Type management (every type has own set of fields)
// Returns all types ids
std::vector<type_id_t>
crypto_kernel::types() const
{
	std::vector<type_id_t> res;
	re.reserve(this->types_.size());
	for (auto &x: this->types_) res.push_back(x.first);
	return res;
}

// Returns type id if it exists or empty string
std::string
crypto_kernel::type(type_id_t tid) const
{
	try {
		return this->types_.at(tid);	// Throws std::out_of_range exception
										// if tid does not exists
	} catch(...) {
		return "";
	}
}

// Generates new type id and adds it with name
type_id_t
crypto_kernel::add_type(const std::string &type_name)
{
	if (type_name.empty()) return invalid_type_id;
	
	// Searching for existing type name
	for (const auto &x: this->types_)
		if (x.second.name == type_name)
			return invalid_type_id;
	
	// Inserting data into types_
	auto p = this->types_.insert_random(std::move({ .name = type_name }));
	if (p.second) return p.first->first;
	else return invalid_type_id;
}

// Sets new name for existing type or returns invalid_type_id
type_id_t
crypto_kernel::set_type(type_id_t tid, const std::string &type_name)
{
	if (type_name.empty()) return invalid_type_id;
	
	auto it = this->types_.find(tid);
	if (it == this->types_.end())
		return invalid_type_id;
	it->second = type_name;
}
// End of type management


// Field management (field ids are unique for same type)
// Returns all fields ids for type or empty vector
std::vector<field_id_t>
crypto_kernel::fields(type_id_t tid) const
{}

// Returns field id if it exists or empty string
std::string
crypto_kernel::field(type_id_t tid, field_id_t fid) const
{}

// Generates new field id and adds it with name
field_id_t
crypto_kernel::add_field(type_id_t tid, const std::string &field_name)
{}

// Sets new name for existing field of existing type or returns invalid_field_id
field_id_t
crypto_kernel::set_field(type_id_t tid, field_id_t fid, const std::string &field_name)
{}
// End of field management
