// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef RANDOM_MAP_H
#define RANDOM_MAP_H

#include <unordered_map>

template<class Key,					// Key type
		 class Value,				// Value Type
		 class Generator,			// Generator, that provides random data of type Key
		 class Container = std::unordered_map<Key, Value>>	// Assotiative container<Key, Value, ContainerArgs>
class random_map: private Container
{
public:
	// Iterators
	using typename Container::iterator;
	using typename Container::const_iterator;
	
	using Container::begin;
	using Container::end;
	
	// Other functions from Container
	using Container::find;
	using Container::at;
	using Container::size;
	using Container::erase;
	
	// Constructors and operator=()
	inline random_map(const Generator &g = Generator());							// Default
	inline random_map(Generator &&g);												// By moving generator
	inline random_map(const random_map<Key, Value, Generator, Container> &other);	// Copy
	inline random_map(random_map<Key, Value, Generator, Container> &&other);		// Move
	
	random_map<Key, Value, Generator, Container> & operator=(const random_map<Key, Value, Generator, Container> &other);	// Copy
	random_map<Key, Value, Generator, Container> & operator=(random_map<Key, Value, Generator, Container> &&other);			// Move
	
	// Other functions
	// Inserts new (key, value) pair, where key is random.
	// Returns iterator to newly inserted pair.
	// TIME: depends on Container::find() and Container::emplace().
	std::pair<typename random_map<Key, Value, Generator, Container>::iterator, bool> insert(const Value &val);
	std::pair<typename random_map<Key, Value, Generator, Container>::iterator, bool> insert(Value &&val);
	
	inline const Generator & generator() const;
private:
	Generator generator_;
};

#include "random_map.hpp"
#endif // RANDOM_MAP_H
