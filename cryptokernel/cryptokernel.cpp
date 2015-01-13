// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernel.h"

#include <queue>
#include "io.h"

#define SPACE "\t"

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
bool
cryptokernel::read(std::istream &s)
{
	this->clear();
	try {
		// types_map: old_type_id -> (new_type_id, old_tfield_id -> new_tfield_id)
		std::unordered_map<type_id_t,							
						   std::pair<type_id_t,					
						   			 std::unordered_map<tfield_id_t,
						   			 					tfield_id_t>>> types_map;
		
		// groups_map: old_group_id -> new_group_id
		std::unordered_map<group_id_t, group_id_t> groups_map;
		
		// records_map: old_record_id -> new_record_id
		std::unordered_map<record_id_t, record_id_t> records_map;
		
		// Reading metadata
		{
			size_t types_count;
			
			// Global data
			io::read(s, types_count);	// Number of types
			this->types_order_.reserve(types_count);
			
			while (types_count--) {	// Types
				type_id_t old_type_id;
				std::string type_name;
				size_t fields_count;
				tfields_order_t tfields_order;
				
				// Type data
				io::read(s, old_type_id);	// Id
				io::read(s, type_name);		// Name
				io::read(s, fields_count);	// Number of fields
				
				auto new_type_id = this->add_type(type_name);
				if (new_type_id == invalid_type_id) throw (io::exception());
				
				auto &type_data = types_map[old_type_id];
				type_data.first = new_type_id;
				
				while (fields_count--) {	// Fields of type
					tfield_id_t old_tfield_id;
					std::string field_name, field_format;
					
					// Field data
					io::read(s, old_tfield_id);	// Id
					io::read(s, field_name);	// Name
					io::read(s, field_format);	// Format
					
					auto new_tfield_id = this->add_type_field(new_type_id, field_name, field_format);
					if (new_tfield_id == invalid_tfield_id) throw (io::exception());
					type_data.second[old_tfield_id] = new_tfield_id;
				}
			}
		}
		// Metadata read
		
		
		// Reading data
		// Reading groups
		do {	// Cycle do { ... } while (false) -- breaks if no groups exist
			auto read_subgroups = [&s, this, &groups_map](group_id_t group_id) {
				size_t subgroups_count;
				
				io::read(s, subgroups_count);
				
				while (subgroups_count--) {
					group_id_t old_subgroup_id;
					std::string subgroup_name;
					
					io::read(s, old_subgroup_id);
					io::read(s, subgroup_name);
					
					group_id_t new_subgroup_id = this->add_group(group_id, subgroup_name);
					if (new_subgroup_id == invalid_group_id) throw (io::exception());
					groups_map[old_subgroup_id] = new_subgroup_id;
				}
			};
			
			auto read_subrecords = [&s, this, &records_map, &types_map](group_id_t group_id) {
				size_t subrecords_count;
				
				io::read(s, subrecords_count);
				
				while (subrecords_count--) {
					record_id_t old_subrecord_id;
					std::string subrecord_name;
					
					io::read(s, old_subrecord_id);
					io::read(s, subrecord_name);
					
					// invalid_type_id here is hack: change it later, when record data arrived
					record_id_t new_subrecord_id = this->add_record(group_id,
																	subrecord_name,
																	invalid_type_id);
					if (new_subrecord_id == invalid_record_id) throw (io::exception());
					records_map[old_subrecord_id] = new_subrecord_id;
				}
			};
			
			
			size_t groups_count;
			group_id_t old_root_group_id;
			
			// Global data
			io::read(s, old_root_group_id);	// Root group id
			io::read(s, groups_count);		// Number of groups
			
			// No groups exist => no records exist
			if (old_root_group_id == invalid_group_id || groups_count == 0)
				break;
			
			groups_map[invalid_group_id] = invalid_group_id;
			
			if (groups_count--) {	// Root group (always first)
				group_id_t group_id;
				
				// Group data
				io::read(s, group_id);	// Id
				
				if (group_id != old_root_group_id)
					throw (io::exception());
				
				add_root_group();
				if (this->root_group_id_ == invalid_group_id) throw (io::exception());
				group_id = groups_map[old_root_group_id] = this->root_group_id_;
				
				groups_map[group_id] = this->root_group_id_;
				
				read_subgroups(group_id);	// Child groups
				read_subrecords(group_id);	// Child records
			}
			
			while (groups_count--) {	// Other groups
				group_id_t group_id;
				
				// Group data
				io::read(s, group_id);	// Id
				
				group_id = groups_map[group_id];
				
				read_subgroups(group_id);	// Child groups
				read_subrecords(group_id);	// Child records
			}
		} while (false);
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
				record_id_t record_id;
				type_id_t record_type_id;
				size_t fields_count;
				
				// Record data
				io::read(s, record_id);			// Id
				io::read(s, record_type_id);	// Type of record
				io::read(s, fields_count);		// Number of fields
				
				// Updating record type and id
				auto &type_pair = types_map.at(record_type_id);
				record_type_id = type_pair.first;
				
				record_id = records_map.at(record_id);
				this->records_.at(record_id).type = record_type_id;
				
				while (fields_count--) {	// Fields
					rfield_id_t field_id;
					tfield_id_t field_type_id;
					std::string field_data;
					
					// Field data
					io::read(s, field_id);
					io::read(s, field_type_id);
					io::read(s, field_data);
					
					field_type_id = type_pair.second.at(field_type_id);
					
					auto new_field_id = this->add_field(record_id, field_type_id, field_data);
					if (new_field_id == invalid_rfield_id) throw (io::exception());
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
			
			std::queue<group_id_t> groups_queue;		// Queue for BFS
			groups_queue.push(this->root_group_id_);	// Start from root group
			
			while (!groups_queue.empty()) {
				group_id_t group_id = groups_queue.front();
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
// End of types management


// Fields management (field ids are unique for same type)
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
cryptokernel::type_field_format(type_id_t tid, tfield_id_t fid) const
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
// End of fields management
// End of metadata management


// Data management
// Records and groups management
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

// Returns all records of type tid
std::vector<record_id_t>
cryptokernel::records_of_type(type_id_t tid) const
{
	std::vector<record_id_t> res;
	if (!this->test_type(tid)) return res;
	
	// Search for records of type tid (linear time)
	for (const auto &record: this->records_)
		if (record.second.type == tid)
			res.push_back(record.first);
	return res;
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
	if (record_name.empty() || this->root_group_id_ == invalid_group_id
		|| (!this->types_.test1(record_type) && record_type != invalid_type_id))
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
	if (group_name.empty() || this->root_group_id_ == invalid_group_id)
		return invalid_group_id;
	try {
		auto &groups = this->groups_.at(gid).groups;
		if (groups.test2(group_name)) return invalid_group_id;
		
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
	return invalid_group_id;
}

// Generates new root group id and adds it or returns invalid_group_id
group_id_t
cryptokernel::add_root_group()
{
	if (this->root_group_id_ != invalid_group_id)
		return invalid_group_id;
	// Inserting data into groups_
	auto p = this->groups_.insert_random(this->group_generator_,
										 { .parent_group = invalid_group_id });
	if (p.second) {
		this->root_group_id_ = p.first->first;
		return this->root_group_id_;
	}
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
	if (gid == this->root_group_id_) {
		this->groups_.clear();
		this->records_.clear();
		return gid;
	}
	
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


// Returns record id if it exists or invalid_type_id
type_id_t
cryptokernel::record_type(record_id_t rid) const
{
	try {
		return this->records_.at(rid).type;
	} catch (...) {}
	return invalid_type_id;
}

// Sets new type for existing record if if does not contain fields
// or returns invalid_record_id
record_id_t
cryptokernel::set_record_type(record_id_t rid, type_id_t record_type_id)
{
	try {
		auto &record_data = this->records_.at(rid);
		if (record_data.fields.empty() && this->test_type(record_type_id)) {
			record_data.type = record_type_id;
			return rid;
		}
	} catch (...) {}
	return invalid_record_id;
}


// Returns records parent group id if record exists or invalid_group_id
group_id_t
cryptokernel::record_parent_group(record_id_t rid) const
{
	try {
		return this->records_.at(rid).parent_group;
	} catch (...) {}
	return invalid_group_id;
}

// Returns groups parent group id if group exists or invalid_group_id
group_id_t
cryptokernel::group_parent_group(group_id_t gid) const
{
	try {
		return this->groups_.at(gid).parent_group;
	} catch (...) {}
	return invalid_group_id;
}


// Sets new parent group for existing record or returns invalid_record_id
record_id_t
cryptokernel::set_record_parent_group(record_id_t rid, group_id_t parent_group_id)
{
	try {
		auto &record_parent_group = this->records_.at(rid).parent_group;
		if (this->test_group(parent_group_id)) {
			record_parent_group = parent_group_id;
			return rid;
		}
	} catch (...) {}
	return invalid_record_id;
}

// Sets new parent group for existing group or returns invalid_group_id
group_id_t
cryptokernel::set_group_parent_group(group_id_t gid, group_id_t parent_group_id)
{
	try {
		auto &group_parent_group = this->groups_.at(gid).parent_group;
		if (this->test_group(parent_group_id)) {
			group_parent_group = parent_group_id;
			return gid;
		}
	} catch (...) {}
	return invalid_group_id;
}
// End of records and groups management


// Fields management (field ids are unique for same record)
// Returns all fields ids for record (ids are ordered by user) or empty vector
// Returns number of records
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
		if (p.second) {
			record.fields_order.push_back(p.first->first);
			return p.first->first;
		}
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
cryptokernel::field_type(record_id_t rid, rfield_id_t fid) const
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
cryptokernel::field_data(record_id_t rid, rfield_id_t fid) const
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
	this->root_group_id_ = invalid_group_id;
}


// Checks groups, returns false if no groups exist, true otherwise
bool
cryptokernel::groups_exist() const
{
	if (this->root_group_id_ == invalid_group_id || this->groups_.empty())
		return false;
	return true;
}
