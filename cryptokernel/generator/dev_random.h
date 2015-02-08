// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains classes getting random data from /dev/random-device.
// The default device is DEV_RANDOM_DEVICE.
// All these class are final, do NOT write subclasses overriding their operator()'s.
// All 3 classes are thread-sefety and async-safety.
// NOTE: all dev_random generators use single stream assotiated with DEV_RANDOM!

#ifndef DEV_RANDOM_H
#define DEV_RANDOM_H

#include <fstream>
#include <string>
#include <mutex>

#include "generator.h"

#define DEV_RANDOM_DEVICE "/dev/random"

namespace generator {

class raw_dev_random:
	public base,
	public raw
{
public:
	// Constructors and operator=()
	raw_dev_random();												// Default
	inline raw_dev_random(const raw_dev_random &other);				// Copy
	inline raw_dev_random(raw_dev_random &&other);					// Move
	
	inline raw_dev_random & operator=(const raw_dev_random &other);	// Copy
	inline raw_dev_random & operator=(raw_dev_random &&other);		// Move
	
	virtual void operator()(void *data, size_t n) const override final;
private:
	static std::ifstream stream_;
	static std::mutex mutex_;
};


class universal_dev_random:
	public raw_dev_random,
	public universal
{
public:
	// Constructors and operator=()
	inline universal_dev_random();												// Default
	inline universal_dev_random(const universal_dev_random &other);				// Copy
	inline universal_dev_random(universal_dev_random &&other);					// Move
	
	inline universal_dev_random & operator=(const universal_dev_random &other);	// Copy
	inline universal_dev_random & operator=(universal_dev_random &&other);		// Move
	
	template<class Num>
	inline Num generate() const;	// Generates any number (if id, it may be incorrect, remebmer to check!)
	
	template<class Num, class MappingType = double>
	Num generate(const Num &min, const Num &max) const;	// Generates number in [min, max] diapasone
	
	template<class Num>
	inline const universal_dev_random & operator>>(Num &n) const;	// Same as generate<Num>()
private:
	using raw_dev_random::operator();
};


template<class Num, class MappingType = double>
class dev_random:
	public raw_dev_random,
	public simple<Num>
{
public:
	// Constructors and operator=()
	inline dev_random();									// Default
	inline dev_random(const dev_random<Num, MappingType> &other);		// Copy
	inline dev_random(dev_random<Num, MappingType> &&other);				// Move
	
	inline dev_random & operator=(const dev_random &other);	// Copy
	inline dev_random & operator=(dev_random &&other);		// Move
	
	virtual Num operator()() const override final;	// Generates any number (if id, it may be incorrect, use id_generator!)
	virtual Num operator()(const Num &min, const Num &max) const override final;	// Generates number in [min, max] diapasone
private:
	using raw_dev_random::operator();
};

}; // namespace generator


#include "dev_random.hpp"
#endif // DEV_RANDOM_H
