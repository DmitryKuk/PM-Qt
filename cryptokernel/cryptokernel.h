// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This is the kernel of this program. No frameworks here! Only STL.
// cryptokernel class uses random_map, double_map and
// generator::dev_random<T> for managing types and their fields.

// NOTE: type, record and their fields ids are NOT const, sometimes
// cryptokernel changes them: on every program start.

#ifndef CRYPTOKERNEL_H
#define CRYPTOKERNEL_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "types.h"
#include "random_map.h"
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
	// Reads all data from given stream. If any error occured,
	// don't erase old data
	bool read(std::istream &s);	// NOT IMPLEMENTED YET
	
	// Writes all data to given stream
	bool write(std::ostream &s) const;	// NOT IMPLEMENTED YET
  // End of input/output functions
	
	
  // Metadata management
  // Types management (every type has own set of fields)
	// Returns all types ids (ids are ordered by user)
	std::vector<types::type_id> types() const;
	
	
	// Return true if type exists, false otherwise
	bool test_type(types::type_id tid) const;
	
	// Generates new type id and adds it with name
	types::type_id add_type(const std::string &type_name);
	
	// Removes existing type with its fields,
	// resetting records of this type and their fields to invalid
	types::type_id remove_type(types::type_id tid);
	
	
	// Returns type id if it exists or empty string
	std::string type_name(types::type_id tid) const;
	
	// Sets new name for existing type or returns invalid_type_id
	types::type_id set_type_name(types::type_id tid, const std::string &type_name);
  // End of types management
	
	
  // Fields management (field ids are unique for same type)
	// Returns all fields ids for type (ids are ordered by user) or empty vector
	std::vector<types::tfield_id> type_fields(types::type_id tid) const;
	
	
	// Returns true if type and field exist, false otherwise
	bool test_type_field(types::type_id tid, types::tfield_id fid) const;
	
	// Generates new field id and adds it with name
	types::tfield_id add_type_field(types::type_id tid, const std::string &field_name, const std::string &field_format = "");
	
	// Removes existing type field, resetting all records' fields of this type to invalid
	types::tfield_id remove_type_field(types::type_id tid, types::tfield_id fid);
	
	
	// Returns type field name if it exists or "", if not
	std::string type_field_name(types::type_id tid, types::tfield_id fid) const;
	
	// Sets new name for existing field of existing type or returns invalid_tfield_id
	types::tfield_id set_type_field_name(types::type_id tid, types::tfield_id fid, const std::string &field_name);
	
	
	// Returns field format, if field fid exists in type tid or ""
	std::string type_field_format(types::type_id tid, types::tfield_id fid) const;
	
	// Sets field format, if field fid exists in type tid or returns invalid_tfield_id
	types::tfield_id set_type_field_format(types::type_id tid, types::tfield_id fid, const std::string &format);
  // End of fields management
  // End of metadata management
	
	
  // Data management
  // Records and groups management
	// Returns root group id
	types::group_id root_group_id() const;
	
	
	// Returns all records ids
	std::vector<types::record_id> records() const;
	
	// Returns all records ids in group gid
	std::vector<types::record_id> records(types::group_id gid) const;
	
	// Returns all records of type tid
	std::vector<types::record_id> records_of_type(types::type_id tid) const;
	
	
	// Returns all groups ids
	std::vector<types::group_id> groups() const;
	
	// Returns all groups ids in group gid
	std::vector<types::group_id> groups(types::group_id gid) const;
	
	
	// Return true if record exists, false otherwise
	bool test_record(types::record_id rid) const;
	
	// Return true if group exists, false otherwise
	bool test_group(types::group_id gid) const;
	
	
	// Generates new record id and adds it with name into existing group
	// or returns invalid_record_id
	types::record_id add_record(types::group_id gid, const std::string &record_name, types::type_id record_type);
	
	// Generates new group id and adds it with name into existing group
	// or returns invalid_group_id
	types::group_id add_group(types::group_id gid, const std::string &group_name);
	
	// Generates new root group id and adds it or returns invalid_group_id
	types::group_id add_root_group();
	
	
	// Removes existing record or returns invalid_record_id
	types::record_id remove_record(types::record_id rid);
	
	// Removes existing group or returns invalid_group_id
	types::group_id remove_group(types::group_id gid);
	
	
	// Returns record id if it exists or empty string
	std::string record_name(types::record_id rid) const;
	
	// Returns group id if it exists or empty string
	std::string group_name(types::group_id gid) const;
	
	
	// Sets new name for existing record or returns invalid_record_id
	types::record_id set_record_name(types::record_id rid, const std::string &record_name);
	
	// Sets new name for existing group or returns invalid_group_id
	types::group_id set_group_name(types::group_id gid, const std::string &group_name);
	
	
	// Returns record type id if record exists or invalid_type_id
	types::type_id record_type(types::record_id rid) const;
	
	// Sets new type for existing record if if does not contain fields
	// or returns invalid_record_id
	types::record_id set_record_type(types::record_id rid, types::type_id record_type_id);
	
	
	// Returns records parent group id if record exists or invalid_group_id
	types::group_id record_parent_group(types::record_id rid) const;
	
	// Returns groups parent group id if group exists or invalid_group_id
	types::group_id group_parent_group(types::group_id gid) const;
	
	
	// Sets new parent group for existing record or returns invalid_record_id
	types::record_id set_record_parent_group(types::record_id rid, types::group_id parent_group_id);
	
	// Sets new parent group for existing group or returns invalid_group_id
	types::group_id set_group_parent_group(types::group_id gid, types::group_id parent_group_id);
  // End of records and groups management
	
  // Fields management (field ids are unique for same record)
	// Returns all fields ids for record (ids are ordered by user) or empty vector
	std::vector<types::rfield_id> fields(types::record_id rid) const;
	
	
	// Returns true if record and field exist, false otherwise
	bool test_field(types::record_id rid, types::rfield_id fid) const;
	
	// Generates new field id and adds it with name
	types::rfield_id add_field(types::record_id rid, types::tfield_id type, const std::string &data = "");
	
	// Removes existing field
	types::rfield_id remove_field(types::record_id rid, types::rfield_id fid);
	
	// Returns field type, if field fid exists in record rid, or invalid_type_id
	types::tfield_id field_type(types::record_id rid, types::rfield_id fid) const;
	
	// Sets field type or returns invalid_tfield_id, if it is impossible
	types::rfield_id set_field_type(types::record_id rid, types::rfield_id fid, types::tfield_id type);
	
	
	// Returns field data, if field fid exists in record rid, or ""
	std::string field_data(types::record_id rid, types::rfield_id fid) const;
	
	// Sets field data, if field fid exists in record rid, or returns invalid_tfield_id
	types::rfield_id set_field_data(types::record_id rid, types::rfield_id fid, const std::string &data);
  // End of fields management
  // End of data management
	
	
	// Clears all data and metadata
	void clear();
private:
	// Checks groups, returns false if no groups exist, true otherwise
	bool groups_exist() const;
	
	
  // Generators
	// Default generators types
	typedef generator::id_generator<generator::dev_random, types::type_id	> type_generator;
	typedef generator::id_generator<generator::dev_random, types::tfield_id	> tfield_generator_t;
	typedef generator::id_generator<generator::dev_random, types::record_id	> record_generator;
	typedef generator::id_generator<generator::dev_random, types::rfield_id	> rfield_generator;
	typedef generator::id_generator<generator::dev_random, types::group_id	> group_generator;
	
	type_generator type_generator_;			// Default type ids generator
	tfield_generator_t tfield_generator_;	// Default type field ids generator
	record_generator record_generator_;		// Default record ids generator
	rfield_generator rfield_generator_;		// Default record field ids generator
	group_generator group_generator_;		// Default group ids generator
  // End of generators
	
  // Types and fields containers
	struct tfield	// Type field
	{
		std::string format;
	};
	
	struct type	// Type
	{
		// Fields double map: field id, field name: tfield
		typedef double_map<types::tfield_id, std::string, tfield> tfields_dmap_t;
		
		tfields_dmap_t fields;
	};
	
	// Types double map: type id, type name: type
	typedef double_map<types::type_id, std::string, type> types_dmap_t;
	
	types_dmap_t types_;	// Types map
  // End of types and fields containers
	
  // Types and fields orders containers
	typedef std::vector<types::tfield_id> tfields_order_t;
	typedef std::vector<std::pair<types::type_id, tfields_order_t>> types_order_t;
	
	types_order_t types_order_;
  // End of types and fields orders containers
	
	
  // Data containers
	struct rfield	// Record field
	{
		types::tfield_id type;	// This fields type
		std::string data;	// Data this field contains
	};
	
	struct record	// Record
	{
		// Fields map: field id: rfield
		typedef random_map<types::rfield_id, rfield> rfields_rmap;
		
		types::type_id type;						// Type this record is of
		types::group_id parent_group;				// Group containing this record
		rfields_rmap fields;						// This records fields
		std::vector<types::rfield_id> fields_order;	// This records fields order container
	};
	
	// Groups and records hierarchy
	struct group	// Group
	{
		// Records double map: record id, record name: record data
		typedef double_map<types::record_id, std::string, types::record_id> records_dmap;
		
		// Groups double map: group id, group name: group data
		typedef double_map<types::group_id, std::string, types::group_id> groups_dmap;
		
		types::group_id parent_group;	// Group containing this record
		groups_dmap groups;				// Groups this group contain
		records_dmap records;			// Records this group contain
	};
	
	// Global records and groups containers
	typedef random_map<types::record_id, record> records_rmap;
	typedef random_map<types::group_id, group> groups_rmap;
	
	records_rmap records_;
	groups_rmap groups_;
	types::group_id root_group_id_;
  // End of data containers
	
	
  // Deprecated members
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy crypto_kernel!")]]
	cryptokernel(const cryptokernel &other);				// Copy
	
	[[deprecated("Don't copy crypto_kernel!")]]
	cryptokernel & operator=(const cryptokernel &other);	// Copy
  // End of deprecated members
};

#endif // CRYPTOKERNEL_H
