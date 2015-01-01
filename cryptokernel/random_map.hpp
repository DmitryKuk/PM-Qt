// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Constructors and operator=()
// Default
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Container, ContainerArgs...>::random_map()
{}

// Copy
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Container, ContainerArgs...>::random_map(const random_map<Key, Value, Container, ContainerArgs...> &other):
	Container<Key, Value, ContainerArgs...>(other)
{}

// Move
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
random_map<Key, Value, Container, ContainerArgs...>::random_map(random_map<Key, Value, Container, ContainerArgs...> &&other):
	Container<Key, Value, ContainerArgs...>(other)
{}


// Copy
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
inline
random_map<Key, Value, Container, ContainerArgs...> &
random_map<Key, Value, Container, ContainerArgs...>::operator=(const random_map<Key, Value, Container, ContainerArgs...> &other)
{
	this->Container<Key, Value, ContainerArgs...>::operator=(other);
	return *this;
}

// Move
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
inline
random_map<Key, Value, Container, ContainerArgs...> &
random_map<Key, Value, Container, ContainerArgs...>::operator=(random_map<Key, Value, Container, ContainerArgs...> &&other)
{
	this->Container<Key, Value, ContainerArgs...>::operator=(std::move(other));
	return *this;
}


// Other functions
// Inserts new (key, value) pair, where key is random.
// Returns iterator to newly inserted pair.
// TIME: depends on Container::find() and Container::emplace().
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
template<class Generator>
std::pair<typename random_map<Key, Value, Container, ContainerArgs...>::iterator, bool>
random_map<Key, Value, Container, ContainerArgs...>::insert_random(const Generator &g, const Value &val)
{
	iterator end = this->end();
	while (true) {
		Key k = g();
		if (g.bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container<Key, Value, ContainerArgs...>::emplace(std::move(k), val);
			return p;
		}
	}
}

template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
template<class Generator>
std::pair<typename random_map<Key, Value, Container, ContainerArgs...>::iterator, bool>
random_map<Key, Value, Container, ContainerArgs...>::insert_random(const Generator &g, Value &&val)
{
	iterator end = this->end();
	while (true) {
		Key k = g();
		if (g.bad())
			return std::make_pair(end, false);
		iterator it = this->find(k);
		if (it == end) {
			auto p = this->Container<Key, Value, ContainerArgs...>::emplace(std::move(k), std::move(val));
			return p;
		}
	}
}


// Updates element key and returns true. If is is impossible (k does not exist
// or new_k exists), returns false.
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
bool
random_map<Key, Value, Container, ContainerArgs...>::update(const Key &k, const Key &new_k)
{
	auto it = this->find(&k);
	if (it == this->end()) return false;
	
	auto p = this->insert(std::move(std::make_pair(&new_k, it->second)));
	if (p.second) {	// Inserted
		this->erase(it);	// Cleaning old data
		return true;
	}
	return false;
}


// Returns true, if key k exists, false otherwise
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
inline
bool
random_map<Key, Value, Container, ContainerArgs...>::test(const Key &k) const
{
	auto it = this->find(k);
	if (it == this->end()) return false;
	return true;
}


// Returns vector with all keys in map
template<class Key, class Value,
		 template<class K, class V, class...>
		 class Container,
		 class ...ContainerArgs>
std::vector<Key>
random_map<Key, Value, Container, ContainerArgs...>::all_keys() const
{
	std::vector<Key> res;
	res.reserve(this->size());
	for (const auto &x: *this)
		res.push_back(x.first);
	return res;
}
