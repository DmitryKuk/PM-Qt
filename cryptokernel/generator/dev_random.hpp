// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Raw
// Constructors and operator=()
inline
generator::raw_dev_random::raw_dev_random(const generator::raw_dev_random &other):	// Copy
	generator::base(other)
{}

inline
generator::raw_dev_random::raw_dev_random(generator::raw_dev_random &&other):		// Move
	generator::base(std::move(other))
{}

inline
generator::raw_dev_random &
generator::raw_dev_random::operator=(const generator::raw_dev_random &other)		// Copy
{
	this->generator::base::operator=(other);
	return *this;
}

inline
generator::raw_dev_random &
generator::raw_dev_random::operator=(generator::raw_dev_random &&other)				// Move
{
	this->generator::base::operator=(std::move(other));
	return *this;
}


// Universal
// Constructors and operator=()
inline
generator::universal_dev_random::universal_dev_random():												// Default
	generator::raw_dev_random()
{}

inline
generator::universal_dev_random::universal_dev_random(const generator::universal_dev_random &other):	// Copy
	generator::raw_dev_random(other)
{}

inline
generator::universal_dev_random::universal_dev_random(generator::universal_dev_random &&other):			// Move
	generator::raw_dev_random(std::move(other))
{}

inline
generator::universal_dev_random &
generator::universal_dev_random::operator=(const generator::universal_dev_random &other)				// Copy
{
	this->generator::raw_dev_random::operator=(other);
	return *this;
}

inline
generator::universal_dev_random &
generator::universal_dev_random::operator=(generator::universal_dev_random &&other)						// Move
{
	this->generator::raw_dev_random::operator=(std::move(other));
	return *this;
}


template<class Num>
inline
Num
generator::universal_dev_random::generate() const
{ return this->generate<Num>(std::numeric_limits<Num>::min(), std::numeric_limits<Num>::max()); }

template<class Num, class MappingType>
Num generator::universal_dev_random::generate(const Num &min, const Num &max) const	// Generates number in [min, max] diapasone
{
	if (min >= max) return min;
	
	Num res(min);
	(*this)(static_cast<void *>(&res), sizeof(Num));	// Generating using the raw form
	if (this->bad()) return min;
	
	// Mapping [global_min, global_max] (for example: [0, 4E+9]) into local [min, max] diapasone:
	// min + (res - global_min) * local_len / global_len
	constexpr Num global_min = std::numeric_limits<Num>::min(),
				  global_max = std::numeric_limits<Num>::max();
	MappingType global_len = static_cast<MappingType>(global_max - global_min),
				local_len  = static_cast<MappingType>(max - min);
	
	return static_cast<Num>(min + ((res - global_min) * local_len) / global_len);
}


template<class Num>
inline
const generator::universal_dev_random &
generator::universal_dev_random::operator>>(Num &n) const
{
	n = this->generate<Num>();
	return *this;
}


// Simple
// Constructors and operator=()
template<class Num, class MappingType>
inline
generator::dev_random<Num, MappingType>::dev_random():
	generator::raw_dev_random()
{}

template<class Num, class MappingType>
inline
generator::dev_random<Num, MappingType>::dev_random(const generator::dev_random<Num, MappingType> &other):	// Copy
	generator::raw_dev_random(other)
{}

template<class Num, class MappingType>
inline
generator::dev_random<Num, MappingType>::dev_random(generator::dev_random<Num, MappingType> &&other):		// Move
	generator::raw_dev_random(std::move(other))
{}

template<class Num, class MappingType>
inline
generator::dev_random<Num, MappingType> &
generator::dev_random<Num, MappingType>::operator=(const generator::dev_random<Num, MappingType> &other)	// Copy
{
	this->generator::raw_dev_random::operator=(other);
	return *this;
}

template<class Num, class MappingType>
inline
generator::dev_random<Num, MappingType> &
generator::dev_random<Num, MappingType>::operator=(generator::dev_random<Num, MappingType> &&other)			// Move
{
	this->generator::raw_dev_random::operator=(std::move(other));
	return *this;
}


template<class Num, class MappingType>
Num
generator::dev_random<Num, MappingType>::operator()() const	// Generates any number (if id, it may be incorrect, remebmer to check!)
{ return (*this)(std::numeric_limits<Num>::min(), std::numeric_limits<Num>::max()); }

template<class Num, class MappingType>
Num
generator::dev_random<Num, MappingType>::operator()(const Num &min, const Num &max) const	// Generates number in [min, max] diapasone
{
	if (min >= max) return min;
	
	Num res(min);
	(*this)(static_cast<void *>(&res), sizeof(Num));	// Generating using the raw form
	if (this->bad()) return min;
	
	// Mapping [global_min, global_max] (for example: [0, 4E+9]) into local [min, max] diapasone:
	// min + (res - global_min) * local_len / global_len
	constexpr Num global_min = std::numeric_limits<Num>::min(),
				  global_max = std::numeric_limits<Num>::max();
	MappingType global_len = static_cast<MappingType>(global_max - global_min),
				local_len  = static_cast<MappingType>(max - min);
	
	return static_cast<Num>(min + ((res - global_min) * local_len) / global_len);
}
