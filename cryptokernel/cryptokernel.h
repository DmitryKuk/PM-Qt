// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This is the kernel of this program. No frameworks here! Only STL.
// cryptokernel class uses random_map, double_map and
// generator::dev_random<T> for managing types and their fields.

// NOTE: type and field ids are NOT const, sometimes cryptokernel
// changes them: on every program start or more often.

// TODO: write double_set class and replace some double_maps by it

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
	int read(std::istream &s);	// NOT IMPLEMENTED YET
	
	// Writes all data to given stream
	int write(std::ostream &s) const;	// NOT IMPLEMENTED YET
  // End of input/output functions
	
	
  // Metadata management
  // Type management (every type has own set of fields)
	// Returns all types ids (ids are ordered by user)
	std::vector<type_id_t> types() const;
	
	
	// Return true if type exists, false otherwise
	bool test_type(type_id_t tid) const;
	
	// Generates new type id and adds it with name
	type_id_t add_type(const std::string &type_name);
	
	// Removes existing type with its fields
	type_id_t remove_type(type_id_t tid);
	
	
	// Returns type id if it exists or empty string
	std::string type_name(type_id_t tid) const;
	
	// Sets new name for existing type or returns invalid_type_id
	type_id_t set_type_name(type_id_t tid, const std::string &type_name);
  // End of type management
	
	
  // Field management (field ids are unique for same type)
	// Returns all fields ids for type (ids are ordered by user) or empty vector
	std::vector<tfield_id_t> type_fields(type_id_t tid) const;
	
	
	// Returns true if type and field exist, false otherwise
	bool test_type_field(type_id_t tid, tfield_id_t fid) const;
	
	// Generates new field id and adds it with name
	tfield_id_t add_type_field(type_id_t tid, const std::string &field_name, const std::string &field_format = "");
	
	// Removes existing field
	tfield_id_t remove_type_field(type_id_t tid, tfield_id_t fid);
	
	
	// Returns type field name if it exists or "", if not
	std::string type_field_name(type_id_t tid, tfield_id_t fid) const;
	
	// Sets new name for existing field of existing type or returns invalid_tfield_id
	tfield_id_t set_type_field_name(type_id_t tid, tfield_id_t fid, const std::string &field_name);
	
	
	// Returns field format, if field fid exists in type tid or ""
	std::string type_field_format(type_id_t tid, tfield_id_t fid);
	
	// Sets field format, if field fid exists in type tid or returns invalid_tfield_id
	tfield_id_t set_type_field_format(type_id_t tid, tfield_id_t fid, const std::string &format);
  // End of field management
  // End of metadata management
	
	
  // Data management
  // Records and groups management
	// Returns root group id
	group_id_t root_group_id() const;
	
	
	// Returns all records ids
	std::vector<record_id_t> records() const;
	
	// Returns all records ids in group gid
	std::vector<record_id_t> records(group_id_t gid) const;
	
	// Returns all groups ids
	std::vector<group_id_t> groups() const;
	
	// Returns all groups ids in group gid
	std::vector<record_id_t> groups(group_id_t gid) const;
	
	
	// Return true if record exists, false otherwise
	bool test_record(record_id_t rid) const;
	
	// Return true if group exists, false otherwise
	bool test_group(group_id_t gid) const;
	
	
	// Generates new record id and adds it with name into existing group
	// or returns invalid_record_id
	record_id_t add_record(group_id_t gid, const std::string &record_name, type_id_t record_type);
	
	// Generates new group id and adds it with name into existing group
	// or returns invalid_group_id
	group_id_t add_group(group_id_t gid, const std::string &group_name);
	
	
	// Removes existing record or returns invalid_record_id
	record_id_t remove_record(record_id_t rid);
	
	// Removes existing group or returns invalid_group_id
	group_id_t remove_group(group_id_t gid);
	
	
	// Returns record id if it exists or empty string
	std::string record_name(record_id_t rid) const;
	
	// Returns group id if it exists or empty string
	std::string group_name(group_id_t gid) const;
	
	
	// Sets new name for existing record or returns invalid_record_id
	record_id_t set_record_name(record_id_t rid, const std::string &record_name);
	
	// Sets new name for existing group or returns invalid_group_id
	group_id_t set_group_name(group_id_t gid, const std::string &group_name);
  // End of records and groups management
	
  // Field management (field ids are unique for same record)
	// Returns all fields ids for record (ids are ordered by user) or empty vector
	std::vector<rfield_id_t> fields(record_id_t rid) const;
	
	
	// Returns true if record and field exist, false otherwise
	bool test_field(record_id_t rid, rfield_id_t fid) const;
	
	// Generates new field id and adds it with name
	rfield_id_t add_field(record_id_t rid, tfield_id_t type, const std::string &data = "");
	
	// Removes existing field
	rfield_id_t remove_field(record_id_t rid, rfield_id_t fid);
	
	// Returns field type, if field fid exists in record rid, or invalid_type_id
	tfield_id_t field_type(record_id_t rid, rfield_id_t fid);
	
	// Sets field type or returns invalid_tfield_id, if it is impossible
	rfield_id_t set_field_type(record_id_t rid, rfield_id_t fid, tfield_id_t type);
	
	
	// Returns field data, if field fid exists in record rid, or ""
	std::string field_data(record_id_t rid, rfield_id_t fid);
	
	// Sets field data, if field fid exists in record rid, or returns invalid_tfield_id
	rfield_id_t set_field_data(record_id_t rid, rfield_id_t fid, const std::string &data);
  // End of field management
  // End of data management
private:
  // Generators
	// Default generators types
	typedef generator::dev_random<type_id_t> type_generator_t;
	typedef generator::dev_random<tfield_id_t> tfield_genertor_t;
	typedef generator::dev_random<record_id_t> record_generator_t;
	typedef generator::dev_random<rfield_id_t> rfield_generator_t;
	typedef generator::dev_random<group_id_t> group_generator_t;
	
	type_generator_t type_generator_;		// Default type ids generator
	tfield_genertor_t tfield_generator_;	// Default type field ids generator
	record_generator_t record_generator_;	// Default record ids generator
	rfield_generator_t rfield_generator_;	// Default record field ids generator
	group_generator_t group_generator_;		// Default group ids generator
  // End of generators
	
  // Types and fields containers
	struct tfield_t	// Type field
	{
		std::string format;
	};
	
	struct type_t	// Type
	{
		// Fields double map: field id, field name: tfield_t
		typedef double_map<tfield_id_t, std::string, tfield_t> tfields_dmap_t;
		
		tfields_dmap_t fields;
	};
	
	// Types double map: type id, type name: type_t
	typedef double_map<type_id_t, std::string, type_t> types_dmap_t;
	
	types_dmap_t types_;	// Types map
  // End of types and fields containers
	
  // Types and fields orders containers
	typedef std::vector<tfield_id_t> tfields_order_t;
	typedef std::vector<std::pair<type_id_t, tfields_order_t>> types_order_t;
	
	types_order_t types_order_;
  // End of types and fields orders containers
	
	
  // Data containers
	struct rfield_t	// Record field
	{
		tfield_id_t type;	// This fields type
		std::string data;	// Data this field contains
	};
	
	struct record_t	// Record
	{
		// Fields map: field id: rfield_t
		typedef random_map<rfield_id_t, rfield_t> rfields_rmap_t;
		
		type_id_t type;							// Type this record is of
		group_id_t parent_group;				// Group containing this record
		rfields_rmap_t fields;					// This records fields
		std::vector<rfield_id_t> fields_order;	// This records fields order container
	};
	
	// Groups and records hierarchy
	struct group_t	// Group
	{
		// Records double map: record id, record name: record data
		typedef double_map<record_id_t, std::string, record_id_t> records_dmap_t;
		
		// Groups double map: group id, group name: group data
		typedef double_map<group_id_t, std::string, group_id_t> groups_dmap_t;
		
		group_id_t parent_group;	// Group containing this record
		groups_dmap_t groups;		// Groups this group contain
		records_dmap_t records;		// Records this group contain
	};
	
	// Global records and groups containers
	typedef random_map<record_id_t, record_t> records_rmap_t;
	typedef random_map<group_id_t, group_t> groups_rmap_t;
	
	records_rmap_t records_;
	groups_rmap_t groups_;
	group_id_t root_group_id_;
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
