// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains classes getting random data from /dev/random-device.
// The default device is DEV_RANDOM.
// All these class are final, do NOT write subclasses overriding their generate()
// functions. All 3 classes are thread-sefety and async-safety.

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
	raw_dev_random(const std::string &dev_name = DEV_RANDOM);
	
	virtual void generate(void *data, size_t n) const override final;
private:
	mutable std::ifstream stream_;
	mutable std::mutex mutex_;
};


class universal_dev_random:
	virtual public universal,
	virtual private raw_dev_random
{
public:
	universal_dev_random(const std::string &dev_name = DEV_RANDOM);
	
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
	dev_random(const std::string &dev_name = DEV_RANDOM);
	
	virtual Num generate() const override final;
private:
	using raw_dev_random::generate;
};

}; // namespace generator


template<class Num>
inline const generator::universal_dev_random & operator>>(const generator::universal_dev_random &g, Num &n);


#include "dev_random.hpp"
#undef DEV_RANDOM
#endif // DEV_RANDOM_H
