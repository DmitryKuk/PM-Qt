// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "generator.h"

extern const int
	generator::status::not_initialized		= 0,	// Generator was not be initialized correctly
	generator::status::success				= 10,	// Operation was successful
	generator::status::not_enough_entropy	= 20,	// Operation was incorrect because of entropy pool is empty
	generator::status::unknown_error		= 30;	// Another error occured

// Base
generator::base::~base()
{}


// Raw
generator::raw::~raw()
{}


// Universal
generator::universal::~universal()
{}
