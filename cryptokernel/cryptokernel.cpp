// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernel.h"

#include <algorithm>

// Constructors, destructor and operator=()
cryptokernel::cryptokernel():						// Default
	root_group_id_(invalid_group_id)
{}

cryptokernel::cryptokernel(cryptokernel &&other):	// Move
	type_generator_(std::move(other.type_generator_)),
	tfield_generator_(std::move(other.tfield_generator_)),
	record_generator_(std::move(other.record_generator_)),
	rfield_generator_(std::move(other.rfield_generator_)),
	group_generator_(std::move(other.group_generator_)),
	
	types_(std::move(other.types_)),
	types_order_(std::move(other.types_order_)),
	
	records_(std::move(other.records_)),
	groups_(std::move(other.groups_)),
	root_group_id_(std::move(other.root_group_id_))
{}

cryptokernel::~cryptokernel()
{}


cryptokernel &
cryptokernel::operator=(cryptokernel &&other)		// Move
{
	this->type_generator_	= std::move(other.type_generator_);
	this->tfield_generator_	= std::move(other.tfield_generator_);
	this->record_generator_	= std::move(other.record_generator_);
	this->rfield_generator_	= std::move(other.rfield_generator_);
	this->group_generator_	= std::move(other.group_generator_);
	
	this->types_			= std::move(other.types_);
	this->types_order_		= std::move(other.types_order_);
	
	this->records_			= std::move(other.records_);
	this->groups_			= std::move(other.groups_);
	this->root_group_id_	= std::move(other.root_group_id_);
	
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


// Metadata management
// Type management (every type has own set of fields)
// Returns all types ids (ids are ordered by user)
std::vector<type_id_t>
cryptokernel::types() const
{
	std::vector<type_id_t> res;
	res.reserve(this->types_order_.size());
	for (auto &x: this->types_order_)
		res.push_back(x.first);
	return res;
}


// Return true if type exists, false otherwise
bool
cryptokernel::test_type(type_id_t tid) const
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return false;	// Not found
	return true;
}

// Generates new type id and adds it with name
type_id_t
cryptokernel::add_type(const std::string &type_name)
{
	if (type_name.empty()) return invalid_type_id;
	
	// Inserting data into types_
	auto p = this->types_.insert_random1(this->type_generator_, type_name, type_t());
	if (p.second) {
		this->types_order_.emplace_back(p.first->first, std::move(tfields_order_t()));
		return p.first->first;
	}
	return invalid_type_id;
}

// Removes existing type with its fields
type_id_t
cryptokernel::remove_type(type_id_t tid)
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end())
		return invalid_type_id;	// Type does not exist
	this->types_.erase(it);	// Erasing type with its fields from types_
	
	// Erasing type from type order
	for (auto order_it = this->types_order_.begin(), order_end = this->types_order_.end();
			order_it != order_end;
			++order_it)
		if (order_it->first == tid) {
			this->types_order_.erase(order_it);
			break;
		}
	return tid;
}


// Returns type id if it exists or empty string
std::string
cryptokernel::type_name(type_id_t tid) const
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return "";	// Not found
	return it->second;
}

// Sets new name for existing type or returns invalid_type_id
type_id_t
cryptokernel::set_type_name(type_id_t tid, const std::string &type_name)
{
	if (type_name.empty() || !this->types_.update12(tid, type_name))
		return invalid_type_id;
	return tid;
}
// End of type management


// Field management (field ids are unique for same type)
// Returns all fields ids for type (ids are ordered by user) or empty vector
std::vector<tfield_id_t>
cryptokernel::type_fields(type_id_t tid) const
{
	for (const auto &x: this->types_order_)
		if (x.first == tid)
			return x.second;
	return std::vector<tfield_id_t>();
}


// Return true if type and field exist, false otherwise
bool
cryptokernel::test_type_field(type_id_t tid, tfield_id_t fid) const
{
	auto type_it = this->types_.find1(tid);
	if (type_it == this->types_.end()) return false;	// Type not found
	
	auto field_it = type_it->third.fields.find1(fid);
	if (field_it == type_it->third.fields.end()) return false;	// Field not found
	return true;
}

// Generates new field id and adds it with name
tfield_id_t
cryptokernel::add_type_field(type_id_t tid, const std::string &field_name, const std::string &field_format)
{
	if (field_name.empty()) return invalid_tfield_id;
	
	try {
		auto &fields = this->types_.at1(tid).fields;
		auto p = fields.insert_random1(this->tfield_generator_,
									   field_name,
									   { .format = field_format });
		if (p.second) {
			// Updating order
			for (auto &x: this->types_order_)	// Searching for tid in types_order_
				if (x.first == tid) {
					x.second.push_back(p.first->first);	// Adding new field to order
					break;
				}
			return p.first->first;
		}
	} catch (...) {}
	return invalid_tfield_id;
}

// Removes existing field
tfield_id_t
cryptokernel::remove_type_field(type_id_t tid, tfield_id_t fid)
{
	try {
		this->types_.at1(tid).fields.erase1(fid);
	} catch (...) {
		return invalid_tfield_id;
	}
	
	// Erasing field from type order
	for (auto &p: this->types_order_)
		if (p.first == tid) {
			for (auto field_order_it = p.second.begin(), field_order_end = p.second.end();
					field_order_it != field_order_end;
					++field_order_it)
				if (*field_order_it == fid) {
					p.second.erase(field_order_it);
					break;
				}
			break;
		}
	return fid;
}


// Returns type field name if it exists or "", if not
std::string
cryptokernel::type_field_name(type_id_t tid, tfield_id_t fid) const
{
	try {
		const auto &fields = this->types_.at1(tid).fields;
		auto it = fields.find1(fid);
		if (it == fields.end()) return "";
		return it->second;
	} catch (...) {}
	return "";
}

// Sets new name for existing field of existing type or returns invalid_tfield_id
tfield_id_t
cryptokernel::set_type_field_name(type_id_t tid, tfield_id_t fid, const std::string &field_name)
{
	if (field_name.empty()) return invalid_tfield_id;
	
	try {
		auto &fields = this->types_.at1(tid).fields;
		if (fields.update12(fid, field_name)) return fid;
	} catch (...) {}
	return invalid_tfield_id;
}


// Returns field data, if field fid exists in type tid or ""
std::string
cryptokernel::type_field_format(type_id_t tid, tfield_id_t fid)
{
	try {
		return this->types_.at1(tid).fields.at1(fid).format;
	} catch (...) {}
	return "";
}

// Sets field data, if field fid exists in type tid or returns invalid_tfield_id
tfield_id_t
cryptokernel::set_type_field_format(type_id_t tid, tfield_id_t fid, const std::string &format)
{
	try {
		this->types_.at1(tid).fields.at1(fid).format = format;
		return fid;
	} catch (...) {}
	return invalid_tfield_id;
}
// End of field management
// End of metadata management


// Records management
// Returns root group id
group_id_t
cryptokernel::root_group_id() const
{
	return this->root_group_id_;
}


// Returns all records ids (ids are ordered by user)
std::vector<record_id_t>
cryptokernel::records() const
{
	return this->records_.all_keys();
}

// Returns all records ids in group gid
std::vector<record_id_t>
cryptokernel::records(group_id_t gid) const
{
	try {
		return this->groups_.at(gid).records.all_keys1();
	} catch (...) {}
	return std::vector<record_id_t>();
}

// Returns all groups ids
std::vector<group_id_t>
cryptokernel::groups() const
{
	return this->groups_.all_keys();
}

// Returns all groups ids in group gid
std::vector<group_id_t>
cryptokernel::groups(group_id_t gid) const
{
	try {
		return this->groups_.at(gid).groups.all_keys1();
	} catch (...) {}
	return std::vector<group_id_t>();
}


// Return true if record exists, false otherwise
bool
cryptokernel::test_record(record_id_t rid) const
{
	auto it = this->records_.find(rid);
	if (it == this->records_.end()) return false;	// Not found
	return true;
}

// Return true if group exists, false otherwise
bool
cryptokernel::test_group(group_id_t gid) const
{
	auto it = this->groups_.find(gid);
	if (it == this->groups_.end()) return false;	// Not found
	return true;
}


// Generates new record id and adds it with name
record_id_t
cryptokernel::add_record(group_id_t gid, const std::string &record_name, type_id_t record_type)
{
	if (record_name.empty() || !this->types_.test1(record_type))
		return invalid_record_id;
	try {
		auto &records = this->groups_.at(gid).records;
		if (records.test2(record_name)) return invalid_record_id;
		
		// Inserting data into records_
		auto p = this->records_.insert_random(this->record_generator_,
											  { .type = record_type,
											  	.parent_group = gid });
		if (p.second) {
			records.insert(std::move(make_triple(p.first->first,
												 record_name,
												 p.first->first)));
			return p.first->first;
		}
	} catch (...) {}
	return invalid_record_id;
}

// Generates new group id and adds it with name into existing group
// or returns invalid_group_id
group_id_t
cryptokernel::add_group(group_id_t gid, const std::string &group_name)
{
	if (group_name.empty()) return invalid_group_id;
	try {
		auto &groups = this->groups_.at(gid).groups;
		if (groups.test2(group_name)) return invalid_group_id;
		
		// Inserting data into groups_
		auto p = this->groups_.insert_random(this->group_generator_,
											 { .parent_group = gid, });
		if (p.second) {
			groups.insert(std::move(make_triple(p.first->first,
												group_name,
												p.first->first)));
			if (this->root_group_id_ == invalid_group_id)	// Updating root group id
				this->root_group_id_ = p.first->first;
			return p.first->first;
		}
	} catch (...) {}
	return invalid_group_id;
}


// Removes existing record
record_id_t
cryptokernel::remove_record(record_id_t rid)
{
	auto record_it = this->records_.find(rid);
	if (record_it == this->records_.end())
		return invalid_record_id;	// Record does not exist
	
	try {	// Erasing record from its parent group
		this->groups_.at(record_it->second.parent_group).records.erase1(rid);
	} catch (...) {}
	
	// Erasing record with its fields from records_
	this->records_.erase(record_it);
	return rid;
}

// Removes existing group or returns invalid_group_id
group_id_t
cryptokernel::remove_group(group_id_t gid)
{
	auto group_it = this->groups_.find(gid);
	if (group_it == this->groups_.end())
		return invalid_group_id;	// Group does not exist
	
	try {	// Erasing group from its parent group
		this->groups_.at(group_it->second.parent_group).groups.erase1(gid);
	} catch (...) {}
	
	// Erasing groups child records and groups
	for (auto &x: group_it->second.records) this->remove_record(x.first);
	for (auto &x: group_it->second.groups) this->remove_group(x.first);
	
	// Erasing record with its fields from groups_
	this->groups_.erase(group_it);
	if (this->root_group_id_ == gid)	// Updating root group id
		this->root_group_id_ = invalid_group_id;
	return gid;
}


// Returns record id if it exists or empty string
std::string
cryptokernel::record_name(record_id_t rid) const
{
	auto it = this->records_.find(rid);
	if (it == this->records_.end()) return "";	// Not found
	try {
		auto &records = this->groups_.at(it->second.parent_group).records;
		auto it1 = records.find1(rid);
		if (it1 == records.end()) return "";
		return it1->second;
	} catch (...) {}
	return "";
}

// Returns group id if it exists or empty string
std::string
cryptokernel::group_name(group_id_t gid) const
{
	auto it = this->groups_.find(gid);
	if (it == this->groups_.end()) return "";	// Not found
	try {
		auto &groups = this->groups_.at(it->second.parent_group).groups;
		auto it1 = groups.find1(gid);
		if (it1 == groups.end()) return "";
		return it1->second;
	} catch (...) {}
	return "";
}


// Sets new name for existing record or returns invalid_record_id
record_id_t
cryptokernel::set_record_name(record_id_t rid, const std::string &record_name)
{
	if (record_name.empty()) return invalid_record_id;
	try {
		auto &dmap = this->groups_.at(this->records_.at(rid).parent_group).records;
		if (dmap.update12(rid, record_name)) return rid;
	} catch (...) {}
	return invalid_record_id;
}

// Sets new name for existing group or returns invalid_group_id
group_id_t
cryptokernel::set_group_name(group_id_t gid, const std::string &group_name)
{
	if (group_name.empty()) return invalid_group_id;
	try {
		auto &dmap = this->groups_.at(this->groups_.at(gid).parent_group).groups;
		if (dmap.update12(gid, group_name)) return gid;
	} catch (...) {}
	return invalid_group_id;
}
// End of records management


// Field management (field ids are unique for same record)
// Returns all fields ids for record (ids are ordered by user) or empty vector
std::vector<rfield_id_t>
cryptokernel::fields(record_id_t rid) const
{
	try {
		return this->records_.at(rid).fields_order;
	} catch (...) {}
	return std::vector<rfield_id_t>();
}
	
	
// Returns true if record and field exist, false otherwise
bool
cryptokernel::test_field(record_id_t rid, rfield_id_t fid) const
{
	try {
		this->records_.at(rid).fields.at(fid);
		return true;
	} catch (...) {}
	return false;
}

// Generates new field id and adds it with name
rfield_id_t
cryptokernel::add_field(record_id_t rid, tfield_id_t type, const std::string &data)
{
	try {
		auto &record = this->records_.at(rid);
		if (!this->test_type_field(record.type, type))
			return invalid_rfield_id;	// Incorrect field type
		auto p = record.fields.insert_random(this->rfield_generator_,
											 { .type = type,
											   .data = data });
		if (p.second) return p.first->first;
	} catch (...) {}
	return invalid_rfield_id;
}

// Removes existing field
rfield_id_t
cryptokernel::remove_field(record_id_t rid, rfield_id_t fid)
{
	try {
		auto &fields = this->records_.at(rid).fields;
		auto it = fields.find(fid);
		if (it != fields.end()) fields.erase(it);
	} catch (...) {}
	return invalid_rfield_id;
}


// Returns field type, if field fid exists in record rid, or invalid_type_id
tfield_id_t
cryptokernel::field_type(record_id_t rid, rfield_id_t fid)
{
	try {
		return this->records_.at(rid).fields.at(fid).type;
	} catch (...) {}
	return invalid_rfield_id;
}

// Sets field type or returns invalid_tfield_id, if it is impossible
rfield_id_t
cryptokernel::set_field_type(record_id_t rid, rfield_id_t fid, tfield_id_t type)
{
	try {
		this->records_.at(rid).fields.at(fid).type = type;
		return fid;
	} catch (...) {}
	return invalid_rfield_id;
}


// Returns field data, if field fid exists in record rid, or ""
std::string
cryptokernel::field_data(record_id_t rid, rfield_id_t fid)
{
	try {
		return this->records_.at(rid).fields.at(fid).data;
	} catch (...) {}
	return "";
}

// Sets field data, if field fid exists in record rid, or returns invalid_tfield_id
rfield_id_t
cryptokernel::set_field_data(record_id_t rid, rfield_id_t fid, const std::string &data)
{
	try {
		this->records_.at(rid).fields.at(fid).data = data;
		return fid;
	} catch (...) {}
	return invalid_rfield_id;
}
