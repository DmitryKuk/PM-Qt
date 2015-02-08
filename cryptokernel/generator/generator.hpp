// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Base
// Constructors, destructor and operator=()
inline
generator::base::base():									// Default
	status_(generator::status::not_initialized)
{}

inline
generator::base::base(const generator::base &other):		// Copy
	status_(std::move(other.status_.load()))	// load() returns temp. object
{}

inline
generator::base::base(generator::base &&other):				// Move
	status_(std::move(other.status_.load(std::memory_order_relaxed)))
{}


inline
generator::base &
generator::base::operator=(const generator::base &other)	// Copy
{
	this->status_ = std::move(other.status_.load());	// load() returns temp. object
	return *this;
}

inline
generator::base &
generator::base::operator=(generator::base &&other)			// Move
{
	this->status_ = std::move(other.status_.load(std::memory_order_relaxed));
	return *this;
}


inline
int
generator::base::status() const
{ return this->status_; }

// Tells about last operation status
inline
bool
generator::base::good() const
{ return this->status() == status::success; }

inline
bool
generator::base::bad() const
{ return !this->good(); }

inline
generator::base::operator bool() const
{ return this->good(); }

// Sets new status. Call this function on every generation
inline
void
generator::base::set_status(int new_status) const
{ this->status_ = new_status; }


// Simple
template<class Num, class MappingType>
generator::simple<Num, MappingType>::~simple()							// Destructor
{}


template<class Num, class MappingType>
inline
const generator::simple<Num, MappingType> &
operator>>(const generator::simple<Num> &g, Num &n)
{
	n = g();
	return g;
}


// Ids generator, see "types.h"
// Constructors
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...>::id_generator(GeneratorArgs &&...args):										// By parameters
	generator_(args...)
{}

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...>::id_generator(const Generator<typename Id::raw_id_type, GeneratorArgs...> &g):	// By generator
	generator_(g)
{}

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...>::id_generator(const id_generator<Generator, Id, GeneratorArgs...> &other):		// Copy
	generator_(other.generator_)
{}

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...>::id_generator(id_generator<Generator, Id, GeneratorArgs...> &&other):			// Move
	generator_(std::move(other.generator_))
{}


// Functions like as generator::base'
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...> &
generator::id_generator<Generator, Id, GeneratorArgs...>::operator=(const generator::id_generator<Generator, Id, GeneratorArgs...> &other)	// Copy
{
	this->generator_ = other.generator_;
	return *this;
}

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...> &
generator::id_generator<Generator, Id, GeneratorArgs...>::operator=(generator::id_generator<Generator, Id, GeneratorArgs...> &&other)		// Move
{
	this->generator_ = std::move(other.generator_);
	return *this;
}

// Returns status of last generation
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
int
generator::id_generator<Generator, Id, GeneratorArgs...>::status() const
{ return this->generator_.status(); }

// Tells about last operation status
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
bool
generator::id_generator<Generator, Id, GeneratorArgs...>::good() const
{ return this->generator_.good(); }

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
bool
generator::id_generator<Generator, Id, GeneratorArgs...>::bad() const
{ return this->generator_.bad(); }

template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
generator::id_generator<Generator, Id, GeneratorArgs...>::operator bool() const
{ return this->generator_; }


// Generates valid id
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
inline
Id
generator::id_generator<Generator, Id, GeneratorArgs...>::operator()() const
{ return (*this)(Id::min(), Id::max()); }

// Generates id in [min, max] diapasone (id may be invalid)
template<template<class Num_, class MappingType_, class...> class Generator, class Id, class ...GeneratorArgs>
Id
generator::id_generator<Generator, Id, GeneratorArgs...>::operator()(const Id &min, const Id &max) const
{
	if (min >= max) return Id::invalid();
	Id res = Id(this->generator_(min.value(), max.value()));	// Generation
	if (this->bad() || res < Id::min() || Id::max() < res)
		return Id::invalid();
	return res;
}
