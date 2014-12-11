// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Constructors and operator=()
// Default
template<class Key, class Value, class Generator, class Container>
random_map<Key, Value, Generator, Container>::random_map(const Generator &g):
	generator_(g)
{}

// Copy
template<class Key, class Value, class Generator, class Container>
random_map<Key, Value, Generator, Container>::random_map(const random_map<Key, Value, Generator, Container> &other):
	Container(other),
	generator_(other.generator_)
{}

// Move
template<class Key, class Value, class Generator, class Container>
random_map<Key, Value, Generator, Container>::random_map(random_map<Key, Value, Generator, Container> &&other):
	Container(other),
	generator_(std::move(other.generator_))
{}


// Copy
template<class Key, class Value, class Generator, class Container>
random_map<Key, Value, Generator, Container> &
random_map<Key, Value, Generator, Container>::operator=(const random_map<Key, Value, Generator, Container> &other)
{
	this->Container::operator=(other);
	this->generator_ = other.generator_;
	return *this;
}

// Move
template<class Key, class Value, class Generator, class Container>
random_map<Key, Value, Generator, Container> &
random_map<Key, Value, Generator, Container>::operator=(random_map<Key, Value, Generator, Container> &&other)
{
	this->Container::operator=(other);
	this->generator_ = std::move(other.generator_);
	return *this;
}


// Other functions
// Inserts new (key, value) pair, where key is random.
// Returns iterator to newly inserted pair.
// TIME: depends on Container::find() and Container::emplace().
template<class Key, class Value, class Generator, class Container>
std::pair<typename random_map<Key, Value, Generator, Container>::iterator, bool>
random_map<Key, Value, Generator, Container>::insert(const Value &val)
{
	iterator end = this->end();
	while (true) {
		Key k = this->generator_.generate();
		if (this->generator_->bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container::emplace(std::move(k), val);
			return p;
		}
	}
}

template<class Key, class Value, class Generator, class Container>
std::pair<typename random_map<Key, Value, Generator, Container>::iterator, bool>
random_map<Key, Value, Generator, Container>::insert(Value &&val)
{
	iterator end = this->end();
	while (true) {
		Key k = this->generator_.generate();
		if (this->generator_.bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container::emplace(std::move(k), std::move(val));
			return p;
		}
	}
}


template<class Key, class Value, class Generator, class Container>
inline
const Generator &
random_map<Key, Value, Generator, Container>::generator() const
{
	return this->generator_;
}
