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


// Simple
template<class Num>
generator::simple<Num>::~simple()							// Destructor
{}


template<class Num>
inline
const generator::simple<Num> &
operator>>(const generator::simple<Num> &g, Num &n)
{
	n = g();
	return g;
}
