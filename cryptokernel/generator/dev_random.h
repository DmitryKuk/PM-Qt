// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains classes getting random data from /dev/random-device.
// The default device is DEV_RANDOM.
// All these class are final, do NOT write subclasses overriding their generate()
// functions. All 3 classes are thread-sefety and async-safety.
// NOTE: all dev_random generators use single stream assotiated with DEV_RANDOM!

#ifndef DEV_RANDOM_H
#define DEV_RANDOM_H

#include <fstream>
#include <string>
#include <mutex>

#include "generator.h"

#define DEV_RANDOM "/dev/random"

namespace generator {

class raw_dev_random:
	virtual public raw
{
public:
	// Constructors and operator=()
	raw_dev_random();												// Default
	inline raw_dev_random(const raw_dev_random &other);				// Copy
	inline raw_dev_random(raw_dev_random &&other);					// Move
	
	inline raw_dev_random & operator=(const raw_dev_random &other);	// Copy
	inline raw_dev_random & operator=(raw_dev_random &&other);		// Move
	
	virtual void generate(void *data, size_t n) const override final;
private:
	static std::ifstream stream_;
	static std::mutex mutex_;
};


class universal_dev_random:
	virtual public universal,
	virtual private raw_dev_random
{
public:
	// Constructors and operator=()
	inline universal_dev_random();												// Default
	inline universal_dev_random(const universal_dev_random &other);				// Copy
	inline universal_dev_random(universal_dev_random &&other);					// Move
	
	inline universal_dev_random & operator=(const universal_dev_random &other);	// Copy
	inline universal_dev_random & operator=(universal_dev_random &&other);		// Move
	
	template<class Num>
	Num generate() const;	// override final
private:
	using raw_dev_random::generate;
};


template<class Num>
class dev_random:
	virtual public simple<Num>,
	virtual private raw_dev_random
{
public:
	// Constructors and operator=()
	inline dev_random();									// Default
	inline dev_random(const dev_random<Num> &other);		// Copy
	inline dev_random(dev_random<Num> &&other);				// Move
	
	inline dev_random & operator=(const dev_random &other);	// Copy
	inline dev_random & operator=(dev_random &&other);		// Move
	
	virtual Num generate() const override final;
private:
	using raw_dev_random::generate;
};

}; // namespace generator


template<class Num>
inline const generator::universal_dev_random & operator>>(const generator::universal_dev_random &g, Num &n);


#include "dev_random.hpp"
#endif // DEV_RANDOM_H
