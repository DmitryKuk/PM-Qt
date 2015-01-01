// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef RANDOM_MAP_H
#define RANDOM_MAP_H

#include <unordered_map>
#include <vector>

template<class Key,					// Key type
		 class Value,				// Value Type
		 template<class K, class V, class...>
		 class Container = std::unordered_map,	// Assotiative container<Key, Value, ContainerArgs>
		 class ...ContainerArgs>	// Additional arguments for container
class random_map: public Container<Key, Value, ContainerArgs...>
{
public:
	typedef Container<Key, Value, ContainerArgs...> parent_container;
	// Iterators
	using typename parent_container::iterator;
	using typename parent_container::const_iterator;
	
	// Constructors and operator=()
	inline random_map();
	inline random_map(const random_map<Key, Value, Container, ContainerArgs...> &other);
	inline random_map(random_map<Key, Value, Container, ContainerArgs...> &&other);
	
	inline random_map<Key, Value, Container, ContainerArgs...> & operator=(const random_map<Key, Value, Container, ContainerArgs...> &other);	// Copy
	inline random_map<Key, Value, Container, ContainerArgs...> & operator=(random_map<Key, Value, Container, ContainerArgs...> &&other);			// Move
	
	// Other functions
	// Inserts new (key, value) pair, where key is random.
	// Returns iterator to newly inserted pair.
	// TIME: depends on Container::find() and Container::emplace().
	template<class Generator>
	std::pair<typename random_map<Key, Value, Container, ContainerArgs...>::iterator, bool> insert_random(const Generator &g, const Value &val);
	
	template<class Generator>
	std::pair<typename random_map<Key, Value, Container, ContainerArgs...>::iterator, bool> insert_random(const Generator &g, Value &&val);
	
	// Updates element key and returns true. If is is impossible (k does not exist
	// or new_k exists), returns false.
	bool update(const Key &k, const Key &new_k);
	
	// Returns true, if key k exists, false otherwise.
	inline bool test(const Key &k) const;
	
	// Returns vector with all keys in map.
	std::vector<Key> all_keys() const;
};

#include "random_map.hpp"
#endif // RANDOM_MAP_H
