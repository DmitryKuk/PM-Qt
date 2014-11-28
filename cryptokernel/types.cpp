// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <limits>

#include "types.h"

// Constants
const type_id_t
	invalid_type_id		= numeric_limits<type_id_t>::max(),
	min_type_id			= numeric_limits<type_id_t>::min(),
	max_type_id			= numeric_limits<type_id_t>::max() - 1;

const field_id_t
	invalid_field_id	= numeric_limits<field_id_t>::max(),
	min_field_id		= numeric_limits<field_id_t>::min(),
	max_field_id		= numeric_limits<field_id_t>::max() - 1;
