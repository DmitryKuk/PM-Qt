// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains class that implements double mapping:
// Key1 -> Value and Ke2 -> Value.
// It is useful when you need to search data with to keys.
// This class uses std::unordered_map for mapping, so all functions work like
// std::unordered_map's. This class uses also std::list for storaging data,
// so all elements are ordered as they were added.

#ifndef DOUBLEMAP_H
#define DOUBLEMAP_H

#include <list>
#include <unordered_map>
#include <vector>

#include "triple.h"

template<class Key1,						class Key2,
		 class Value,
		 class Hash1 = std::hash<Key1>,		class Hash2 = std::hash<Key2>,
		 class Pred1 = std::equal_to<Key1>,	class Pred2 = std::equal_to<Key2>>
class double_map
{
public:
	// Data container
	typedef std::list<triple<Key1, Key2, Value>> container_t;
	
	// Hashers
	struct hash1: private Hash1 {
		inline size_t operator()(const Key1 *k) const;
	};
	
	struct hash2: private Hash2 {
		inline size_t operator()(const Key2 *k) const;
	};
	
	// Predictors (comparators)
	struct pred1: private Pred1 {
		inline bool operator()(const Key1 *k1, const Key1 *k2) const;
	};
	
	struct pred2: private Pred2 {
		inline bool operator()(const Key2 *k1, const Key2 *k2) const;
	};
	
	// Assotiative containers
	typedef std::unordered_map<Key1 const *, typename container_t::iterator, hash1, pred1> map1_t;
	typedef std::unordered_map<Key2 const *, typename container_t::iterator, hash2, pred2> map2_t;
	
	// Iterators
	typedef typename container_t::const_iterator const_iterator;
	inline const_iterator begin() const;
	inline const_iterator end() const;
	
	// Constructors and operator=()
	inline double_map();
	inline double_map(const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &other);
	inline double_map(double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &&other);
	
	inline double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &
	operator=(const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &other);
	
	inline double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &
	operator=(double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &&other);
	
	std::pair<const_iterator, bool> insert(const triple<Key1, Key2, Value> &t);
	std::pair<const_iterator, bool> insert(triple<Key1, Key2, Value> &&t);
	
	// Like random_map::insert_random()
	// Generates Key1 automatically.
	template<class Generator>
	std::pair<const_iterator, bool> insert_random1(const Generator &g, const Key2 &k2, const Value &val);
	
	// Generates Key2 automatically.
	template<class Generator>
	std::pair<const_iterator, bool> insert_random2(const Generator &g, const Key1 &k1, const Value &val);
	
	// Generates both keys automatically.
	template<class Generator>
	std::pair<const_iterator, bool> insert_random(const Generator &g, const Value &val);
	
	void erase(const_iterator it);
	inline void erase1(const Key1 &k1);
	inline void erase2(const Key2 &k2);
	
	// Updates 1st or 2nd key and returns true. If it's impossible (new_k already
	// exists or k1 does not exists), returns false and does NOT change any element.
	bool update1(const Key1 &k1, const Key1 &new_k1);
	bool update2(const Key2 &k2, const Key2 &new_k2);
	
	// As update1/update2, but use one key to update another.
	bool update12(const Key1 &k1, const Key2 &new_k2);
	bool update21(const Key2 &k2, const Key1 &new_k1);
	
	inline size_t size() const;
	
	const_iterator find1(const Key1 &k1) const;
	const_iterator find2(const Key2 &k2) const;
	
	inline Value & at1(const Key1 &k1);
	inline const Value & at1(const Key1 &k1) const;
	
	inline Value & at2(const Key2 &k2);
	inline const Value & at2(const Key2 &k2) const;
	
	// Returns true, if key exists, or false otherwise
	bool test1(const Key1 &k1) const;
	bool test2(const Key2 &k2) const;
	
	// Returns vector with all keys in map
	std::vector<Key1> all_keys1() const;
	std::vector<Key2> all_keys2() const;
private:
	container_t list_;
	map1_t map1_;
	map2_t map2_;
};

#include "double_map.hpp"
#endif // DOUBLEMAP_H
