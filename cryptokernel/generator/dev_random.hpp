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
Num
generator::universal_dev_random::generate() const
{
	Num n(0);
	if (this->status() == status::not_initialized) return n;
	
	this->generate(static_cast<void *>(&n), sizeof(Num));
	return n;
}


// Simple
// Constructors and operator=()
template<class Num>
inline
generator::dev_random<Num>::dev_random():
	generator::raw_dev_random()
{}

template<class Num>
inline
generator::dev_random<Num>::dev_random(const generator::dev_random<Num> &other):	// Copy
	generator::raw_dev_random(other)
{}

template<class Num>
inline
generator::dev_random<Num>::dev_random(generator::dev_random<Num> &&other):			// Move
	generator::raw_dev_random(std::move(other))
{}

template<class Num>
inline
generator::dev_random<Num> &
generator::dev_random<Num>::operator=(const generator::dev_random<Num> &other)		// Copy
{
	this->generator::raw_dev_random::operator=(other);
	return *this;
}

template<class Num>
inline
generator::dev_random<Num> &
generator::dev_random<Num>::operator=(generator::dev_random<Num> &&other)			// Move
{
	this->generator::raw_dev_random::operator=(std::move(other));
	return *this;
}


template<class Num>
Num
generator::dev_random<Num>::generate() const
{
	Num n(0);
	if (this->status() == status::not_initialized) return n;
	
	this->generate(static_cast<void *>(&n), sizeof(Num));
	return n;
}


template<class Num>
inline
const generator::universal_dev_random &
operator>>(const generator::universal_dev_random &g, Num &n)
{
	n = g.generate<Num>();
	return g;
}
