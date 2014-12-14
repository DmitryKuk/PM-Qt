// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Constructors and operator=()
// Default
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...>::random_map(const Generator &g):
	generator_(g)
{}

// By moving generator
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...>::random_map(Generator &&g):
	generator_(std::move(g))
{}

// Copy
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...>::random_map(const random_map<Key, Value, Generator, Container, ContainerArgs...> &other):
	Container<Key, Value, ContainerArgs...>(other),
	generator_(other.generator_)
{}

// Move
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...>::random_map(random_map<Key, Value, Generator, Container, ContainerArgs...> &&other):
	Container<Key, Value, ContainerArgs...>(other),
	generator_(std::move(other.generator_))
{}


// Copy
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...> &
random_map<Key, Value, Generator, Container, ContainerArgs...>::operator=(const random_map<Key, Value, Generator, Container, ContainerArgs...> &other)
{
	this->Container<Key, Value, ContainerArgs...>::operator=(other);
	this->generator_ = other.generator_;
	return *this;
}

// Move
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Generator, Container, ContainerArgs...> &
random_map<Key, Value, Generator, Container, ContainerArgs...>::operator=(random_map<Key, Value, Generator, Container, ContainerArgs...> &&other)
{
	this->Container<Key, Value, ContainerArgs...>::operator=(std::move(other));
	this->generator_ = std::move(std::move(other.generator_));
	return *this;
}


// Other functions
// Inserts new (key, value) pair, where key is random.
// Returns iterator to newly inserted pair.
// TIME: depends on Container::find() and Container::emplace().
template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
std::pair<typename random_map<Key, Value, Generator, Container, ContainerArgs...>::iterator, bool>
random_map<Key, Value, Generator, Container, ContainerArgs...>::insert_random(const Value &val)
{
	iterator end = this->end();
	while (true) {
		Key k = this->generator_.generate();
		if (this->generator_.bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container<Key, Value, ContainerArgs...>::emplace(std::move(k), val);
			return p;
		}
	}
}

template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
std::pair<typename random_map<Key, Value, Generator, Container, ContainerArgs...>::iterator, bool>
random_map<Key, Value, Generator, Container, ContainerArgs...>::insert_random(Value &&val)
{
	iterator end = this->end();
	while (true) {
		Key k = this->generator_.generate();
		if (this->generator_.bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container<Key, Value, ContainerArgs...>::emplace(std::move(k), std::move(val));
			return p;
		}
	}
}


template<class Key, class Value, class Generator,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
inline
const Generator &
random_map<Key, Value, Generator, Container, ContainerArgs...>::generator() const
{
	return this->generator_;
}
