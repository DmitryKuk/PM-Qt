// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Base
// Constructors, destructor and operator=()
inline
generator::base::base():									// Default
	status_(generator::status::not_initialized)
{}

inline
generator::base::base(const generator::base &other):		// Copy
	status_(std::move(other.status_.load()))
{}

inline
generator::base::base(generator::base &&other):				// Move
	status_(std::move(other.status_.load(std::memory_order_relaxed)))
{}


inline
generator::base &
generator::base::operator=(const generator::base &other)	// Copy
{
	this->status_ = other.status_.load();
	return *this;
}

inline
generator::base &
generator::base::operator=(generator::base &&other)			// Move
{
	this->status_ = std::move(other.status_.load(std::memory_order_relaxed));
	return *this;
}


// Raw
// Constructors and operator=()
inline
generator::raw::raw():								// Default
	generator::base()
{}

inline
generator::raw::raw(const generator::raw &other):	// Copy
	generator::base(other)
{}

inline
generator::raw::raw(generator::raw &&other):		// Move
	generator::base(other)
{}

inline
generator::raw &
generator::raw::operator=(const generator::raw &other)			// Copy
{
	this->generator::base::operator=(other);
	return *this;
}

inline
generator::raw &
generator::raw::operator=(generator::raw &&other)				// Move
{
	this->generator::base::operator=(other);
	return *this;
}


// Universal
// Constructors and operator=()
inline
generator::universal::universal():										// Default
	generator::base()
{}

inline
generator::universal::universal(const generator::universal &other):		// Copy
	generator::base(other)
{}

inline
generator::universal::universal(generator::universal &&other):			// Move
	generator::base(other)
{}

inline
generator::universal &
generator::universal::operator=(const generator::universal &other)		// Copy
{
	this->generator::base::operator=(other);
	return *this;
}

inline
generator::universal &
generator::universal::operator=(generator::universal &&other)			// Move
{
	this->generator::base::operator=(other);
	return *this;
}


// Simple
// Constructors, destructor and operator=()
template<class Num>
inline
generator::simple<Num>::simple():										// Default
	generator::base()
{}

template<class Num>
inline
generator::simple<Num>::simple(const generator::simple<Num> &other):	// Copy
	generator::base(other)
{}

template<class Num>
inline
generator::simple<Num>::simple(generator::simple<Num> &&other):			// Move
	generator::base(other)
{}


template<class Num>
generator::simple<Num>::~simple()										// Destructor
{}

template<class Num>
inline
generator::simple<Num> &
generator::simple<Num>::operator=(const generator::simple<Num> &other)	// Copy
{
	this->generator::base::operator=(other);
	return *this;
}

template<class Num>
inline
generator::simple<Num> &
generator::simple<Num>::operator=(generator::simple<Num> &&other)		// Move
{
	this->generator::base::operator=(other);
	return *this;
}


template<class Num>
inline
const generator::simple<Num> &
operator>>(const generator::simple<Num> &g, Num &n)
{
	n = g.generate();
	return g;
}
