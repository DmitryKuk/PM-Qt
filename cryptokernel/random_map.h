// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef RANDOM_MAP_H
#define RANDOM_MAP_H

#include <unordered_map>

template<class Key,					// Key type
		 class Value,				// Value Type
		 class Generator,			// Generator, that provides random data of type Key
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
	inline random_map(const Generator &g = Generator());
	inline random_map(Generator &&g);
	inline random_map(const random_map<Key, Value, Generator, Container, ContainerArgs...> &other);
	inline random_map(random_map<Key, Value, Generator, Container, ContainerArgs...> &&other);
	
	random_map<Key, Value, Generator, Container, ContainerArgs...> & operator=(const random_map<Key, Value, Generator, Container, ContainerArgs...> &other);	// Copy
	random_map<Key, Value, Generator, Container, ContainerArgs...> & operator=(random_map<Key, Value, Generator, Container, ContainerArgs...> &&other);			// Move
	
	// Other functions
	// Inserts new (key, value) pair, where key is random.
	// Returns iterator to newly inserted pair.
	// TIME: depends on Container::find() and Container::emplace().
	std::pair<typename random_map<Key, Value, Generator, Container, ContainerArgs...>::iterator, bool> insert_random(const Value &val);
	std::pair<typename random_map<Key, Value, Generator, Container, ContainerArgs...>::iterator, bool> insert_random(Value &&val);
	
	inline const Generator & generator() const;
private:
	Generator generator_;
};

#include "random_map.hpp"
#endif // RANDOM_MAP_H
