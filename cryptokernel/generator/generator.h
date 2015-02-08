// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains description of basic [pseudo]-random number generator
// classes interfaces (abstract classes).

#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <cstddef>
#include <atomic>

namespace generator {

// Generation status
namespace status {
extern const int
	not_initialized,	// Generator was not be initialized correctly
	success,			// Operation was successful
	not_enough_entropy,	// Operation was incorrect because of entropy pool is empty
	unknown_error;		// Another error occured
};


// Some status management functions
class base
{
public:
	// Constructors, destructor and operator=()
	inline base();					// Default
	inline base(const base &other);	// Copy
	inline base(base &&other);		// Move
	virtual ~base() = 0;
	
	inline generator::base & operator=(const generator::base &other);	// Copy
	inline generator::base & operator=(generator::base &&other);		// Move
	
	// Returns status of last generation
	inline int status() const;
	
	// Tells about last operation status
	inline bool good() const;
	inline bool bad() const;
	inline operator bool() const;
protected:
	// Sets new status. Call this function on every generation
	inline void set_status(int new_status) const;
private:
	mutable std::atomic<int> status_;
};


// Generates some data (n unsigned chars) writing them into array pointed by "data".
class raw
{
public:
	virtual ~raw() = 0;
	
	virtual void operator()(void *data, size_t n) const = 0;
};


// Generates random data of any type.
class universal
{
public:
	virtual ~universal() = 0;
	
	template<class Num>
	Num generate() const;	// Generates any number (if id, it may be incorrect, remebmer to check!)
	
	template<class Num, class MappingType = double>
	Num generate(const Num &min, const Num &max) const;	// Generates number in [min, max] diapasone
	
	template<class Num>
	const universal & operator>>(Num &n) const;	// Same as generate<Num>()
};


// Generates random data of type Num.
template<class Num, class MappingType = double>
class simple
{
public:
	virtual ~simple() = 0;
	
	virtual Num operator()() const = 0;	// Generates any number (if id, it may be incorrect, remebmer to check!)
	virtual Num operator()(const Num &min, const Num &max) const = 0;	// Generates number in [min, max] diapasone
};

};	// namespace generator


template<class Num, class MappingType = double>
inline const generator::simple<Num> & operator>>(const generator::simple<Num> &g, Num &n);


namespace generator {

// Ids generator, see "types.h"
template<template<class Num_, class MappingType_, class...>
		 class Generator,			// Any simple generator
		 class Id,					// See "types.h", universal_id
		 class ...GeneratorArgs>	// Additional arguments for generator
class id_generator
{
public:
	// Constructors
	explicit inline id_generator(GeneratorArgs &&...args);											// By parameters
	explicit inline id_generator(const Generator<typename Id::raw_id_type, GeneratorArgs...> &g);	// By generator
	inline id_generator(const id_generator<Generator, Id, GeneratorArgs...> &other);				// Copy
	inline id_generator(id_generator<Generator, Id, GeneratorArgs...> &&other);						// Move
	
	// Functions like as generator::base'
	inline id_generator<Generator, Id, GeneratorArgs...> & operator=(const id_generator<Generator, Id, GeneratorArgs...> &other);	// Copy
	inline id_generator<Generator, Id, GeneratorArgs...> & operator=(id_generator<Generator, Id, GeneratorArgs...> &&other);		// Move
	
	// Returns status of last generation
	inline int status() const;
	
	// Tells about last operation status
	inline bool good() const;
	inline bool bad() const;
	inline operator bool() const;
	
	
	// Generates valid id
	inline Id operator()() const;
	
	// Generates id in [min, max] diapasone (id may be invalid)
	Id operator()(const Id &min, const Id &max) const;
private:
	Generator<typename Id::raw_id_type, typename Id::mapping_type, GeneratorArgs...> generator_;
};

};	// namespace generator


#include "generator.hpp"
#endif // GENERATOR_H
