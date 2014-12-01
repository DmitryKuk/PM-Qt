// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains data structure "interval set" -- set of
// [numeric] intervals.

// What it can?
// - insert intervals or single objects into set
// - erase intervals or single objects from set
// - for any object of class T answer: is it in the set or not?

// Class T must:
// - has operator++()
// - be compatible with std::set/std::map as a key

#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H

#include <map>

template<class T,
		 class Compare = std::less<T>,
		 class Alloc = std::allocator<std::pair<const T, T>>>
class interval_set: private std::map<T, T, Compare, Alloc>
{
public:
	typedef std::map<T, T, Compare, Alloc> parent_container;
	typedef std::pair<interval_set<T, Compare, Alloc>::iterator, bool> iter_bool_pair;
	
	// Iterators
	class iterator: public parent_container::iterator
	{};
	
	class const_iterator: public parent_container::const_iterator
	{};
	
	inline iterator begin();
	inline const_iterator begin() const;
	inline iterator end();
	inline const_iterator end() const;
	
	// Constructors and operator=()
	inline interval_set(const Compare &key_comp = Compare(), const Alloc &alloc = Alloc());
	inline interval_set(const interval_set<T, Compare, Alloc> &other);
	
	inline interval_set<T, Compare, Alloc> & operator=(const interval_set<T, Compare, Alloc> &other);
	
	// Inserts new interval or element as [element, element + 1) interval and merges it.
	// Correct hint speeds up searching of interval.
	inline iter_bool_pair insert(const T &element, interval_set<T, Compare, Alloc>::iterator hint = this->end());
	inline iter_bool_pair insert(const std::pair<T, T> &interval, interval_set<T, Compare, Alloc>::iterator hint = this->end());
	
	// Removes object from correct interval. If interval comes empty, deletes it.
	// Correct hint speeds up searching of interval.
	void erase(const T &element);
protected:
	// Tries to merge 3 intervals: it and its both left and right neighbours.
	// If merging was successfully, return iterator to the new interval and true.
	// Otherwise, returns given iterator and false.
	inline iter_bool_pair interval_set<T, Compare, Alloc>::merge(interval_set<T, Compare, Alloc>::iterator it);
};

#include "interval_set.hpp"
#endif // INTERVAL_SET_H
