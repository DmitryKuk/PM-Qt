// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <limits>

#include "types.h"

// Constants
// Types
const type_id_t
	invalid_type_id		= std::numeric_limits<type_id_t>::max(),
	min_type_id			= std::numeric_limits<type_id_t>::min(),
	max_type_id			= std::numeric_limits<type_id_t>::max() - 1;

const tfield_id_t
	invalid_tfield_id	= std::numeric_limits<tfield_id_t>::max(),
	min_tfield_id		= std::numeric_limits<tfield_id_t>::min(),
	max_tfield_id		= std::numeric_limits<tfield_id_t>::max() - 1;


// Data records
const record_id_t
	invalid_record_id	= std::numeric_limits<record_id_t>::max(),
	min_record_id		= std::numeric_limits<record_id_t>::min(),
	max_record_id		= std::numeric_limits<record_id_t>::max() - 1;

const rfield_id_t
	invalid_rfield_id	= std::numeric_limits<rfield_id_t>::max(),
	min_rfield_id		= std::numeric_limits<rfield_id_t>::min(),
	max_rfield_id		= std::numeric_limits<rfield_id_t>::max() - 1;

// Data groups
const group_id_t
	invalid_group_id	= std::numeric_limits<group_id_t>::max(),
	min_group_id		= std::numeric_limits<group_id_t>::min(),
	max_group_id		= std::numeric_limits<group_id_t>::max() - 1;
