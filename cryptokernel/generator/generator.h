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
enum class status {
	success,			// Operation was successful
	not_initialized,	// Generator was not be initialized correctly
	not_enough_entropy,	// Operation was incorrect because of entropy pool is empty
	unknown_error		// Another error occured
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
	inline enum status status() const { return this->status_; }
	
	// Tells about last operation status
	inline bool good() const { return this->status() == status::success; }
	inline bool bad() const { return !this->good(); }
	inline operator bool() const { return this->good(); }
protected:
	// Sets new status. Call this function on every generation
	inline void set_status(enum status new_status) const { this->status_ = new_status; }
private:
	mutable std::atomic<enum status> status_;
};


// Generates some data (n unsigned chars) writing them into array pointed by "data".
class raw: virtual public base
{
public:
	// Constructors, destructor and operator=()
	inline raw();					// Default
	inline raw(const raw &other);	// Copy
	inline raw(raw &&other);		// Move
	virtual ~raw() = 0;
	
	inline generator::raw & operator=(const generator::raw &other);	// Copy
	inline generator::raw & operator=(generator::raw &&other);		// Move
	
	virtual void generate(void *data, size_t n) const = 0;
};


// Generates random data of any type.
class universal: virtual public base
{
public:
	// Constructors, destructor and operator=()
	inline universal();							// Default
	inline universal(const universal &other);	// Copy
	inline universal(universal &&other);		// Move
	virtual ~universal() = 0;
	
	inline generator::universal & operator=(const generator::universal &other);	// Copy
	inline generator::universal & operator=(generator::universal &&other);		// Move
	
	template<class Num>
	Num generate() const;
};


// Generates random data of type Num.
template<class Num>
class simple: virtual public base
{
public:
	// Constructors, destructor and operator=()
	inline simple();					// Default
	inline simple(const simple &other);	// Copy
	inline simple(simple &&other);		// Move
	virtual ~simple() = 0;
	
	inline generator::simple<Num> & operator=(const generator::simple<Num> &other);	// Copy
	inline generator::simple<Num> & operator=(generator::simple<Num> &&other);		// Move
	
	virtual Num generate() const = 0;
};

};	// namespace generator


template<class Num>
inline const generator::simple<Num> & operator>>(const generator::simple<Num> &g, Num &n);


#include "generator.hpp"
#endif // GENERATOR_H
