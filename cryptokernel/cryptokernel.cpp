// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernel.h"

// Constructors, destructor and operator=()
cryptokernel::cryptokernel()						// Default
{}

cryptokernel::cryptokernel(cryptokernel &&other):	// Move
	type_generator_(std::move(other.type_generator_)),
	field_generator_(std::move(other.field_generator_)),
	types_(std::move(other.types_))
{}

cryptokernel::~cryptokernel()
{}


cryptokernel &
cryptokernel::operator=(cryptokernel &&other)		// Move
{
	this->type_generator_	= std::move(other.type_generator_);
	this->field_generator_	= std::move(other.field_generator_);
	this->types_			= std::move(other.types_);
	return *this;
}


// Input/output functions
// Reads all data from given stream. If any error occured, don't erase old data
int
cryptokernel::read(std::istream &s)
{
	std::cerr << "cryptokernel::read not implemented yet!" << std::endl;
	return 0;
}

// Writes all data to given stream
int
cryptokernel::write(std::ostream &s) const
{
	std::cerr << "cryptokernel::read not implemented yet!" << std::endl;
	return 0;
}
// End of input/output functions


// Type management (every type has own set of fields)
// Returns all types ids (ids are ordered by user)
std::vector<type_id_t>
cryptokernel::types() const
{
	std::vector<type_id_t> res;
	res.reserve(this->types_.size());
	for (auto &x: this->types_order_)
		res.push_back(x.first);
	return res;
}

// Returns type id if it exists or empty string
std::string
cryptokernel::type(type_id_t tid) const
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return "";	// Not found
	return it->second;
}

// Generates new type id and adds it with name
type_id_t
cryptokernel::add_type(const std::string &type_name)
{
	if (type_name.empty()) return invalid_type_id;
	
	// Inserting data into types_
	auto p = this->types_.insert_random1(this->type_generator_, type_name, fields_dmap_t());
	if (p.second) return p.first->first;
	return invalid_type_id;
}

// Sets new name for existing type or returns invalid_type_id
type_id_t
cryptokernel::set_type(type_id_t tid, const std::string &type_name)
{
	if (type_name.empty()) return invalid_type_id;
	
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return invalid_type_id;
	auto t = *it;
	t.second = type_name;
	this->types_.erase(it);
	auto p = this->types_.insert(t);
	if (p.second) return tid;
	return invalid_type_id;
}
// End of type management


// Field management (field ids are unique for same type)
// Returns all fields ids for type (ids are ordered by user) or empty vector
std::vector<field_id_t>
cryptokernel::fields(type_id_t tid) const
{
	for (const auto &x: this->types_order_)
		if (x.first == tid)
			return x.second;
	return std::vector<field_id_t>();
}

// Returns field id if it exists or empty string
std::string
cryptokernel::field(type_id_t tid, field_id_t fid) const
{
	try {
		const auto &fields = this->types_.at1(tid);
		auto it = fields.find1(fid);
		if (it != fields.end()) return it->second;
	} catch (...) {
		return "";
	}
	return "";
}

// Generates new field id and adds it with name
field_id_t
cryptokernel::add_field(type_id_t tid, const std::string &field_name)
{
	if (field_name.empty()) return invalid_field_id;
	
	try {
		auto &fields = this->types_.at1(tid);
		auto p = fields.insert_random1(this->field_generator_, field_name, "");
		if (p.second) return p.first->first;
	} catch (...) {
		return invalid_field_id;
	}
	return invalid_field_id;
}

// Sets new name for existing field of existing type or returns invalid_field_id
field_id_t
cryptokernel::set_field(type_id_t tid, field_id_t fid, const std::string &field_name)
{
	if (field_name.empty()) return invalid_field_id;
	
	try {
		auto &fields = this->types_.at1(tid);
		auto it1 = fields.find1(fid), end = fields.end();
		if (it1 != end) {			// Key1 exists: OK
			auto it2 = fields.find2(field_name);	// Checking for duplicate field_name
			if (it2 == end) {		// Key2 does not exists: OK
				auto t = *it1;
				t.second = field_name;
				fields.erase(it1);
				auto p = fields.insert(t);
				if (p.second) return fid;
			} else if (it2 == it1)	// Target field has both keys: fid and field_name
				return fid;			// Nothing to change
		}
	} catch (...) {
		return invalid_field_id;
	}
	return invalid_field_id;
}
// End of field management
