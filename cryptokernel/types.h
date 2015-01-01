// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains only simple scalar types and some constants.
// No classes, widgets, items and others!

#ifndef TYPES_H
#define TYPES_H

// Types
typedef unsigned int id_t;	// Identificator type for all objects
typedef id_t tfield_id_t;	// For fields only: "Name", "Phone number", "E-mail"...
typedef id_t type_id_t;		// For types only: "Group", "Password", "Note", "Phone"...
typedef id_t record_id_t;	// For data records only
typedef id_t rfield_id_t;	// For data record fields only
typedef id_t group_id_t;	// For data groups only

// Constants
// Types
extern const type_id_t
	invalid_type_id,	// = numeric_limits<type_id_t>::max()
	min_type_id,		// = numeric_limits<type_id_t>::min()
	max_type_id;		// = numeric_limits<type_id_t>::max() - 1

extern const tfield_id_t
	invalid_tfield_id,	// = numeric_limits<tfield_id_t>::max()
	min_tfield_id,		// = numeric_limits<tfield_id_t>::min()
	max_tfield_id;		// = numeric_limits<tfield_id_t>::max() - 1

// Data records
extern const record_id_t
	invalid_record_id,	// = numeric_limits<record_id_t>::max()
	min_record_id,		// = numeric_limits<record_id_t>::min()
	max_record_id;		// = numeric_limits<record_id_t>::max() - 1

extern const rfield_id_t
	invalid_rfield_id,	// = numeric_limits<rfield_id_t>::max()
	min_rfield_id,		// = numeric_limits<rfield_id_t>::min()
	max_rfield_id;		// = numeric_limits<rfield_id_t>::max() - 1

// Data groups
extern const group_id_t
	invalid_group_id,	// = numeric_limits<group_id_t>::max()
	min_group_id,		// = numeric_limits<group_id_t>::min()
	max_group_id;		// = numeric_limits<group_id_t>::max() - 1

#endif // TYPES_H
