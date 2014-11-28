// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains only simple scalar types and some constants.
// No classes, widgets, items and others!

#ifndef TYPES_H
#define TYPES_H

// Types
typedef unsigned int id_t;	// Identificator type for all objects: passwords, groups...
typedef id_t field_id_t;	// For fields only: "Name", "Phone number", "E-mail"...
typedef id_t type_id_t;		// For types only: "Group", "Password", "Note", "Phone"...

// Constants
extern const type_id_t
	invalid_type_id,	// = numeric_limits<type_id_t>::max()
	min_type_id,		// = numeric_limits<type_id_t>::min()
	max_type_id;		// = numeric_limits<type_id_t>::max() - 1

extern const field_id_t
	invalid_field_id,	// = numeric_limits<field_id_t>::max()
	min_field_id,		// = numeric_limits<field_id_t>::min()
	max_field_id;		// = numeric_limits<field_id_t>::max() - 1

#endif // TYPES_H
