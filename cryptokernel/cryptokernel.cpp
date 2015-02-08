// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernel.h"

#include <queue>
#include "io.h"

#define SPACE "\t"

// Constructors, destructor and operator=()
cryptokernel::cryptokernel()						// Default
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
bool
cryptokernel::read(std::istream &s)
{
	this->clear();
	try {
		// types_map: old_type_id -> (new_type_id, old_tfield_id -> new_tfield_id)
		std::unordered_map<types::type_id,							
						   std::pair<types::type_id,					
						   			 std::unordered_map<types::tfield_id,
						   			 					types::tfield_id>>> types_map;
		
		// groups_map: old_group_id -> new_group_id
		std::unordered_map<types::group_id, types::group_id> groups_map;
		
		// records_map: old_record_id -> new_record_id
		std::unordered_map<types::record_id, types::record_id> records_map;
		
		// Reading metadata
		{
			size_t types_count;
			
			// Global data
			io::read(s, types_count);	// Number of types
			this->types_order_.reserve(types_count);
			
			while (types_count--) {	// Types
				types::type_id old_type_id;
				std::string type_name;
				size_t fields_count;
				tfields_order_t tfields_order;
				
				// Type data
				io::read(s, old_type_id);	// Id
				io::read(s, type_name);		// Name
				io::read(s, fields_count);	// Number of fields
				
				auto new_type_id = this->add_type(type_name);
				if (new_type_id.is_invalid()) throw (io::exception());
				
				auto &type_data = types_map[old_type_id];
				type_data.first = new_type_id;
				
				while (fields_count--) {	// Fields of type
					types::tfield_id old_tfield_id;
					std::string field_name, field_format;
					
					// Field data
					io::read(s, old_tfield_id);	// Id
					io::read(s, field_name);	// Name
					io::read(s, field_format);	// Format
					
					auto new_tfield_id = this->add_type_field(new_type_id, field_name, field_format);
					if (new_tfield_id.is_invalid()) throw (io::exception());
					type_data.second[old_tfield_id] = new_tfield_id;
				}
			}
		}
		// Metadata read
		
		
		// Reading data
		// Reading groups
		{
			// Lambda helpers
			auto read_subgroups = [&s, this, &groups_map](types::group_id group_id) {
				size_t subgroups_count;
				
				io::read(s, subgroups_count);
				
				while (subgroups_count--) {
					types::group_id old_subgroup_id;
					std::string subgroup_name;
					
					io::read(s, old_subgroup_id);
					io::read(s, subgroup_name);
					
					types::group_id new_subgroup_id = this->add_group(group_id, subgroup_name);
					if (new_subgroup_id.is_invalid()) throw (io::exception());
					groups_map[old_subgroup_id] = new_subgroup_id;
				}
			};
			
			auto read_subrecords = [&s, this, &records_map, &types_map](types::group_id group_id) {
				size_t subrecords_count;
				
				io::read(s, subrecords_count);
				
				while (subrecords_count--) {
					types::record_id old_subrecord_id;
					std::string subrecord_name;
					
					io::read(s, old_subrecord_id);
					io::read(s, subrecord_name);
					
					// types::type_id::invalid() here is hack: change it later, when record data arrived
					types::record_id new_subrecord_id = this->add_record(group_id,
																		 subrecord_name,
																		 types::type_id::invalid());
					if (new_subrecord_id.is_invalid()) throw (io::exception());
					records_map[old_subrecord_id] = new_subrecord_id;
				}
			};
			
			
			do {	// Cycle do { ... } while (false) -- breaks if no groups exist
				size_t groups_count;
				types::group_id old_root_group_id;
				
				// Global data
				io::read(s, old_root_group_id);	// Root group id
				io::read(s, groups_count);		// Number of groups
				
				// No groups exist => no records exist
				if (old_root_group_id.is_invalid() || groups_count == 0)
					break;
				
				groups_map[types::group_id::invalid()] = types::group_id::invalid();
				
				if (groups_count--) {	// Root group (always first)
					types::group_id group_id;
					
					// Group data
					io::read(s, group_id);	// Id
					
					if (group_id != old_root_group_id)
						throw (io::exception());
					
					add_root_group();
					if (this->root_group_id_.is_invalid()) throw (io::exception());
					group_id = groups_map[old_root_group_id] = this->root_group_id_;
					
					groups_map[group_id] = this->root_group_id_;
					
					read_subgroups(group_id);	// Child groups
					read_subrecords(group_id);	// Child records
				}
				
				while (groups_count--) {	// Other groups
					types::group_id group_id;
					
					// Group data
					io::read(s, group_id);	// Id
					
					group_id = groups_map[group_id];
					
					read_subgroups(group_id);	// Child groups
					read_subrecords(group_id);	// Child records
				}
			} while (false);
		}
		// Groups read
		
		
		// Reading records
		{
			size_t records_count;
			
			// Global data
			io::read(s, records_count);	// Number of records
			
			// Check: no groups exist, but records exist => incorrect database stream
			if (!this->groups_exist()) {
				if (records_count != 0) throw (io::exception());
			}
			
			while (records_count--) {	// Records
				types::record_id record_id;
				types::type_id type_id;
				size_t fields_count;
				
				// Record data
				io::read(s, record_id);		// Id
				io::read(s, type_id);		// Type of record
				io::read(s, fields_count);	// Number of fields
				
				// Updating record type and id
				auto &type_pair = types_map.at(type_id);
				type_id = type_pair.first;
				
				record_id = records_map.at(record_id);
				this->records_.at(record_id).type = type_id;
				
				while (fields_count--) {	// Fields
					types::rfield_id record_field_id;
					types::tfield_id type_field_id;
					std::string field_data;
					
					// Field data
					io::read(s, record_field_id);
					io::read(s, type_field_id);
					io::read(s, field_data);
					
					if (type_field_id.is_valid())
						type_field_id = type_pair.second.at(type_field_id);
					
					auto new_field_id = this->add_field(record_id, type_field_id, field_data);
					if (new_field_id.is_invalid()) throw (io::exception());
				}
			}
		}
		// Records read
		// Data read
	} catch (...) {
		this->clear();
		return false;
	}
	return true;
}

// Writes all data to given stream
bool
cryptokernel::write(std::ostream &s) const
{
	try {
		// Writing metadata
		{
			auto types_ids = this->types();
			
			// Global data
			io::write(s, types_ids.size());	// Number of types
			s << std::endl;
			
			for (auto type_id: types_ids) {	// Types
				auto fields_ids = this->type_fields(type_id);
				
				// Type data
				s << SPACE;
				io::write(s, type_id);					// Id
				io::write(s, this->type_name(type_id));	// Name
				io::write(s, fields_ids.size());		// Number of fields
				s << std::endl;
				
				for (auto field_id: fields_ids) {	// Fields of type
					// Field data
					s << SPACE SPACE;
					io::write(s, field_id);										// Id
					io::write(s, this->type_field_name(type_id, field_id));		// Name
					io::write(s, this->type_field_format(type_id, field_id));	// Format
					s << std::endl;
				}
			}
			s << std::endl;
		}
		// Metadata wrote
		
		
		// Writing data
		// Writing groups
		do {	// Cycle do { ... } while (false) breaks if no groups exist => no records exist
			// Global data
			io::write(s, this->root_group_id_);	// Root group id
			io::write(s, this->groups_.size());	// Number of groups
			s << std::endl;
			
			// No groups exist
			if (!this->groups_exist()) break;
			
			std::queue<types::group_id> groups_queue;		// Queue for BFS
			groups_queue.push(this->root_group_id_);	// Start from root group
			
			while (!groups_queue.empty()) {
				types::group_id group_id = groups_queue.front();
				groups_queue.pop();
				
				const auto &group_data = this->groups_.at(group_id);
				
				s << SPACE;
				io::write(s, group_id);	// Group id
				s << std::endl;
				
				// Child groups
				s << SPACE SPACE;
				io::write(s, group_data.groups.size());
				s << std::endl;
				
				for (const auto &child_group_triple: group_data.groups) {
					groups_queue.push(child_group_triple.first);
					
					s << SPACE SPACE SPACE;
					io::write(s, child_group_triple.first);		// Child group id
					io::write(s, child_group_triple.second);	// Child group name
					s << std::endl;
				}
				
				// Child records
				s << SPACE SPACE;
				io::write(s, group_data.records.size());
				s << std::endl;
				
				for (const auto &child_record_triple: group_data.records) {
					s << SPACE SPACE SPACE;
					io::write(s, child_record_triple.first);	// Child record id
					io::write(s, child_record_triple.second);	// Child record name
					s << std::endl;
				}
			}
		} while (false);
		// Groups wrote
		
		
		// Writing records
		{
			auto records_ids = this->records();
			bool groups_do_not_exist = !this->groups_exist();
			
			// Global data
			// Check: if groups do not exist, but records do => incorrect cryptokernel data
			if (groups_do_not_exist && !records_ids.empty()) { // It's impossible
				io::write(s, 0);	// Writing correct data: 0 records
				s << std::endl;
				throw (io::exception());
			}
			
			io::write(s, records_ids.size());	// Number of records
			s << std::endl;
			
			for (auto record_id: records_ids) {	// Records
				auto fields_ids = this->fields(record_id);
				
				// Record data
				s << SPACE;
				io::write(s, record_id);					// Id
				io::write(s, this->record_type(record_id));	// Type of record
				io::write(s, fields_ids.size());			// Number of fields
				s << std::endl;
				
				for (auto field_id: fields_ids) {	// Fields
					// Field data
					s << SPACE SPACE;
					io::write(s, field_id);
					io::write(s, this->field_type(record_id, field_id));
					io::write(s, this->field_data(record_id, field_id));
					s << std::endl;
				}
			}
			s << std::endl;
		}
		// Records wrote
		// Data wrote
	} catch (...) {
		return false;
	}
	return true;
}
// End of input/output functions


// Metadata management
// Types management (every type has own set of fields)
// Returns all types ids (ids are ordered by user)
std::vector<types::type_id>
cryptokernel::types() const
{
	std::vector<types::type_id> res;
	res.reserve(this->types_order_.size());
	for (auto &x: this->types_order_)
		res.push_back(x.first);
	return res;
}


// Return true if type exists, false otherwise
bool
cryptokernel::test_type(types::type_id tid) const
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return false;	// Not found
	return true;
}

// Generates new type id and adds it with name
types::type_id
cryptokernel::add_type(const std::string &type_name)
{
	if (type_name.empty()) return types::type_id::invalid();
	
	// Inserting data into types_
	auto p = this->types_.insert_random1(this->type_generator_, type_name, cryptokernel::type());
	if (p.second) {
		this->types_order_.emplace_back(p.first->first, std::move(tfields_order_t()));
		return p.first->first;
	}
	return types::type_id::invalid();
}

// Removes existing type with its fields,
// resetting records of this type and their fields to invalid
types::type_id
cryptokernel::remove_type(types::type_id tid)
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end())
		return types::type_id::invalid();	// Type does not exist
	this->types_.erase(it);	// Erasing type with its fields from types_
	
	// Erasing type from type order
	for (auto order_it = this->types_order_.begin(), order_end = this->types_order_.end();
			order_it != order_end;
			++order_it)
		if (order_it->first == tid) {
			this->types_order_.erase(order_it);
			break;
		}
	
	// Resetting all fields of this type to invalid
	for (auto &record_data: this->records_)	// Searching records of type tid
		if (record_data.second.type == tid) {
			record_data.second.type = types::type_id::invalid();	// Resetting records types to invalid
			for (auto &field_data: record_data.second.fields)	// And resetting records' fields to invalid
				field_data.second.type = types::tfield_id::invalid();
		}
	return tid;
}


// Returns type id if it exists or empty string
std::string
cryptokernel::type_name(types::type_id tid) const
{
	auto it = this->types_.find1(tid);
	if (it == this->types_.end()) return "";	// Not found
	return it->second;
}

// Sets new name for existing type or returns types::type_id::invalid()
types::type_id
cryptokernel::set_type_name(types::type_id tid, const std::string &type_name)
{
	if (type_name.empty() || !this->types_.update12(tid, type_name))
		return types::type_id::invalid();
	return tid;
}
// End of types management


// Fields management (field ids are unique for same type)
// Returns all fields ids for type (ids are ordered by user) or empty vector
std::vector<types::tfield_id>
cryptokernel::type_fields(types::type_id tid) const
{
	for (const auto &x: this->types_order_)
		if (x.first == tid)
			return x.second;
	return std::vector<types::tfield_id>();
}


// Return true if type and field exist, false otherwise
bool
cryptokernel::test_type_field(types::type_id tid, types::tfield_id fid) const
{
	auto type_it = this->types_.find1(tid);
	if (type_it == this->types_.end()) return false;	// Type not found
	
	auto field_it = type_it->third.fields.find1(fid);
	if (field_it == type_it->third.fields.end()) return false;	// Field not found
	return true;
}

// Generates new field id and adds it with name
types::tfield_id
cryptokernel::add_type_field(types::type_id tid, const std::string &field_name, const std::string &field_format)
{
	if (field_name.empty()) return types::tfield_id::invalid();
	
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
	return types::tfield_id::invalid();
}

// Removes existing type field, resetting all records' fields of this type to invalid
types::tfield_id
cryptokernel::remove_type_field(types::type_id tid, types::tfield_id fid)
{
	try {
		this->types_.at1(tid).fields.erase1(fid);
	} catch (...) {
		return types::tfield_id::invalid();
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
	
	// Resetting all fields of this type to invalid
	for (auto &record_data: this->records_)	// Searching records of type tid
		if (record_data.second.type == tid)
			for (auto &field_data: record_data.second.fields)	// Searching fields of type fid
				if (field_data.second.type == fid)
					field_data.second.type = types::tfield_id::invalid();	// And resetting them to invalid
	return fid;
}


// Returns type field name if it exists or "", if not
std::string
cryptokernel::type_field_name(types::type_id tid, types::tfield_id fid) const
{
	if (tid != types::type_id::invalid() && fid != types::tfield_id::invalid())
		try {
			const auto &fields = this->types_.at1(tid).fields;
			auto it = fields.find1(fid);
			if (it == fields.end()) return "";
			return it->second;
		} catch (...) {}
	return "";
}

// Sets new name for existing field of existing type or returns types::tfield_id::invalid()
types::tfield_id
cryptokernel::set_type_field_name(types::type_id tid, types::tfield_id fid, const std::string &field_name)
{
	if (!field_name.empty() && tid != types::type_id::invalid() && fid != types::tfield_id::invalid())
		try {
			auto &fields = this->types_.at1(tid).fields;
			if (fields.update12(fid, field_name)) return fid;
		} catch (...) {}
	return types::tfield_id::invalid();
}


// Returns field data, if field fid exists in type tid or ""
std::string
cryptokernel::type_field_format(types::type_id tid, types::tfield_id fid) const
{
	try {
		return this->types_.at1(tid).fields.at1(fid).format;
	} catch (...) {}
	return "";
}

// Sets field data, if field fid exists in type tid or returns types::tfield_id::invalid()
types::tfield_id
cryptokernel::set_type_field_format(types::type_id tid, types::tfield_id fid, const std::string &format)
{
	try {
		this->types_.at1(tid).fields.at1(fid).format = format;
		return fid;
	} catch (...) {}
	return types::tfield_id::invalid();
}
// End of fields management
// End of metadata management


// Data management
// Records and groups management
// Returns root group id
types::group_id
cryptokernel::root_group_id() const
{
	return this->root_group_id_;
}


// Returns all records ids (ids are ordered by user)
std::vector<types::record_id>
cryptokernel::records() const
{
	return this->records_.all_keys();
}

// Returns all records ids in group gid
std::vector<types::record_id>
cryptokernel::records(types::group_id gid) const
{
	try {
		return this->groups_.at(gid).records.all_keys1();
	} catch (...) {}
	return std::vector<types::record_id>();
}

// Returns all records of type tid
std::vector<types::record_id>
cryptokernel::records_of_type(types::type_id tid) const
{
	std::vector<types::record_id> res;
	if (!this->test_type(tid)) return res;
	
	// Search for records of type tid (linear time)
	for (const auto &record: this->records_)
		if (record.second.type == tid)
			res.push_back(record.first);
	return res;
}


// Returns all groups ids
std::vector<types::group_id>
cryptokernel::groups() const
{
	return this->groups_.all_keys();
}

// Returns all groups ids in group gid
std::vector<types::group_id>
cryptokernel::groups(types::group_id gid) const
{
	try {
		return this->groups_.at(gid).groups.all_keys1();
	} catch (...) {}
	return std::vector<types::group_id>();
}


// Return true if record exists, false otherwise
bool
cryptokernel::test_record(types::record_id rid) const
{
	auto it = this->records_.find(rid);
	if (it == this->records_.end()) return false;	// Not found
	return true;
}

// Return true if group exists, false otherwise
bool
cryptokernel::test_group(types::group_id gid) const
{
	auto it = this->groups_.find(gid);
	if (it == this->groups_.end()) return false;	// Not found
	return true;
}


// Generates new record id and adds it with name
types::record_id
cryptokernel::add_record(types::group_id gid, const std::string &record_name, types::type_id record_type)
{
	if (record_name.empty() || this->root_group_id_.is_invalid()
		|| (record_type != types::type_id::invalid() && !this->types_.test1(record_type)))
		return types::record_id::invalid();
	try {
		auto &records = this->groups_.at(gid).records;
		if (records.test2(record_name)) return types::record_id::invalid();
		
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
	return types::record_id::invalid();
}

// Generates new group id and adds it with name into existing group
// or returns types::group_id::invalid()
types::group_id
cryptokernel::add_group(types::group_id gid, const std::string &group_name)
{
	if (group_name.empty() || this->root_group_id_.is_invalid())
		return types::group_id::invalid();
	try {
		auto &groups = this->groups_.at(gid).groups;
		if (groups.test2(group_name)) return types::group_id::invalid();
		
		// Inserting data into groups_
		auto p = this->groups_.insert_random(this->group_generator_,
											 { .parent_group = gid });
		if (p.second) {
			groups.insert(std::move(make_triple(p.first->first,
												group_name,
												p.first->first)));
			return p.first->first;
		}
	} catch (...) {}
	return types::group_id::invalid();
}

// Generates new root group id and adds it or returns types::group_id::invalid()
types::group_id
cryptokernel::add_root_group()
{
	if (this->root_group_id_ != types::group_id::invalid())
		return types::group_id::invalid();
	
	// Inserting data into groups_
	auto p = this->groups_.insert_random(this->group_generator_,
										 { .parent_group = types::group_id::invalid() });
	if (p.second) {
		this->root_group_id_ = p.first->first;
		return this->root_group_id_;
	}
	return types::group_id::invalid();
}


// Removes existing record
types::record_id
cryptokernel::remove_record(types::record_id rid)
{
	auto record_it = this->records_.find(rid);
	if (record_it == this->records_.end())
		return types::record_id::invalid();	// Record does not exist
	
	try {	// Erasing record from its parent group
		this->groups_.at(record_it->second.parent_group).records.erase1(rid);
	} catch (...) {}
	
	// Erasing record with its fields from records_
	this->records_.erase(record_it);
	return rid;
}

// Removes existing group or returns types::group_id::invalid()
types::group_id
cryptokernel::remove_group(types::group_id gid)
{
	if (gid == this->root_group_id_) {
		this->groups_.clear();
		this->records_.clear();
		this->root_group_id_ = types::group_id::invalid();
		return gid;
	}
	
	auto group_it = this->groups_.find(gid);
	if (group_it == this->groups_.end())
		return types::group_id::invalid();	// Group does not exist
	
	try {	// Erasing group from its parent group
		this->groups_.at(group_it->second.parent_group).groups.erase1(gid);
	} catch (...) {}
	
	// Erasing groups child records and groups
	for (auto &x: group_it->second.records) this->remove_record(x.first);
	for (auto &x: group_it->second.groups) this->remove_group(x.first);
	
	// Erasing record with its fields from groups_
	this->groups_.erase(group_it);
	
	return gid;
}


// Returns record id if it exists or empty string
std::string
cryptokernel::record_name(types::record_id rid) const
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
cryptokernel::group_name(types::group_id gid) const
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


// Sets new name for existing record or returns types::record_id::invalid()
types::record_id
cryptokernel::set_record_name(types::record_id rid, const std::string &record_name)
{
	if (record_name.empty()) return types::record_id::invalid();
	try {
		auto &dmap = this->groups_.at(this->records_.at(rid).parent_group).records;
		if (dmap.update12(rid, record_name)) return rid;
	} catch (...) {}
	return types::record_id::invalid();
}

// Sets new name for existing group or returns types::group_id::invalid()
types::group_id
cryptokernel::set_group_name(types::group_id gid, const std::string &group_name)
{
	if (group_name.empty()) return types::group_id::invalid();
	try {
		auto &dmap = this->groups_.at(this->groups_.at(gid).parent_group).groups;
		if (dmap.update12(gid, group_name)) return gid;
	} catch (...) {}
	return types::group_id::invalid();
}


// Returns record id if it exists or types::type_id::invalid()
types::type_id
cryptokernel::record_type(types::record_id rid) const
{
	try {
		return this->records_.at(rid).type;
	} catch (...) {}
	return types::type_id::invalid();
}

// Sets new type for existing record if if does not contain fields
// or returns types::record_id::invalid()
types::record_id
cryptokernel::set_record_type(types::record_id rid, types::type_id record_type_id)
{
	try {
		auto &record_data = this->records_.at(rid);
		if (record_type_id == record_data.type) return rid;
		
		if (record_type_id == types::type_id::invalid() || this->test_type(record_type_id)) {
			for (auto fid: record_data.fields_order)	// Reset fields types to invalid
				this->set_field_type(rid, fid, types::tfield_id::invalid());
			record_data.type = record_type_id;			// Change record type
			return rid;
		}
	} catch (...) {}
	return types::record_id::invalid();
}


// Returns records parent group id if record exists or types::group_id::invalid()
types::group_id
cryptokernel::record_parent_group(types::record_id rid) const
{
	try {
		return this->records_.at(rid).parent_group;
	} catch (...) {}
	return types::group_id::invalid();
}

// Returns groups parent group id if group exists or types::group_id::invalid()
types::group_id
cryptokernel::group_parent_group(types::group_id gid) const
{
	try {
		return this->groups_.at(gid).parent_group;
	} catch (...) {}
	return types::group_id::invalid();
}


// Sets new parent group for existing record or returns types::record_id::invalid()
types::record_id
cryptokernel::set_record_parent_group(types::record_id rid, types::group_id parent_group_id)
{
	try {
		auto &record_parent_group = this->records_.at(rid).parent_group;
		if (this->test_group(parent_group_id)) {
			record_parent_group = parent_group_id;
			return rid;
		}
	} catch (...) {}
	return types::record_id::invalid();
}

// Sets new parent group for existing group or returns types::group_id::invalid()
types::group_id
cryptokernel::set_group_parent_group(types::group_id gid, types::group_id parent_group_id)
{
	try {
		auto &group_parent_group = this->groups_.at(gid).parent_group;
		if (this->test_group(parent_group_id)) {
			group_parent_group = parent_group_id;
			return gid;
		}
	} catch (...) {}
	return types::group_id::invalid();
}
// End of records and groups management


// Fields management (field ids are unique for same record)
// Returns all fields ids for record (ids are ordered by user) or empty vector
// Returns number of records
std::vector<types::rfield_id>
cryptokernel::fields(types::record_id rid) const
{
	try {
		return this->records_.at(rid).fields_order;
	} catch (...) {}
	return std::vector<types::rfield_id>();
}
	
	
// Returns true if record and field exist, false otherwise
bool
cryptokernel::test_field(types::record_id rid, types::rfield_id rfid) const
{
	try {
		this->records_.at(rid).fields.at(rfid);
		return true;
	} catch (...) {}
	return false;
}

// Generates new field id and adds it with name
types::rfield_id
cryptokernel::add_field(types::record_id rid, types::tfield_id type, const std::string &data)
{
	try {
		auto &record = this->records_.at(rid);
		if (type != types::tfield_id::invalid() && !this->test_type_field(record.type, type))
			return types::rfield_id::invalid();	// Incorrect field type
		
		auto p = record.fields.insert_random(this->rfield_generator_,
											 { .type = type,
											   .data = data });
		if (p.second) {
			record.fields_order.push_back(p.first->first);
			return p.first->first;
		}
	} catch (...) {}
	return types::rfield_id::invalid();
}

// Removes existing field
types::rfield_id
cryptokernel::remove_field(types::record_id rid, types::rfield_id rfid)
{
	try {
		auto &record_data = this->records_.at(rid);
		auto it = record_data.fields.find(rfid);
		if (it == record_data.fields.end()) throw (int());
		record_data.fields.erase(it);
		
		for (auto it = record_data.fields_order.begin(), end = record_data.fields_order.end(); it != end; ++it)
			if (*it == rfid) {
				record_data.fields_order.erase(it);
				break;
			}
		return rfid;
	} catch (...) {}
	return types::rfield_id::invalid();
}


// Returns field type, if field rfid exists in record rid, or types::type_id::invalid()
types::tfield_id
cryptokernel::field_type(types::record_id rid, types::rfield_id rfid) const
{
	try {
		return this->records_.at(rid).fields.at(rfid).type;
	} catch (...) {}
	return types::tfield_id::invalid();
}

// Sets field type or returns types::tfield_id::invalid(), if it is impossible
types::rfield_id
cryptokernel::set_field_type(types::record_id rid, types::rfield_id rfid, types::tfield_id tfid)
{
	try {
		auto &record_data = this->records_.at(rid);
		if (record_data.type.is_valid() && tfid.is_valid()
			&& test_type_field(record_data.type, tfid)) {
			record_data.fields.at(rfid).type = tfid;
			return rfid;
		}
	} catch (...) {}
	return types::rfield_id::invalid();
}


// Returns field data, if field fid exists in record rid, or ""
std::string
cryptokernel::field_data(types::record_id rid, types::rfield_id rfid) const
{
	try {
		return this->records_.at(rid).fields.at(rfid).data;
	} catch (...) {}
	return "";
}

// Sets field data, if field rfid exists in record rid, or returns types::tfield_id::invalid()
types::rfield_id
cryptokernel::set_field_data(types::record_id rid, types::rfield_id rfid, const std::string &data)
{
	try {
		this->records_.at(rid).fields.at(rfid).data = data;
		return rfid;
	} catch (...) {}
	return types::rfield_id::invalid();
}
// End of fields management
// End of data management


// Clears all data and metadata
void
cryptokernel::clear()
{
	this->records_.clear();
	this->groups_.clear();
	this->types_.clear();
	this->types_order_.clear();
	this->root_group_id_ = types::group_id::invalid();
}


// Checks groups, returns false if no groups exist, true otherwise
bool
cryptokernel::groups_exist() const
{
	if (this->root_group_id_.is_invalid() || this->groups_.empty())
		return false;
	return true;
}
