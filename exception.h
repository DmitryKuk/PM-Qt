// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains only exceptions used by some storage classes' methods

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class invalid_id: public std::exception
{
public:
	inline invalid_id(): std::exception() {}
};

class invalid_field_id:	public invalid_id {};	// See fieldsstorage.h
class invalid_type_id:	public invalid_id {};	// See typestorage.h

#endif // EXCEPTION_H
