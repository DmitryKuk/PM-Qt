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
	// TIME: O(log N + 1) (amortized) [search + insert + merge]
	inline void insert(const T &element);
	void insert(const std::pair<T, T> &interval);
	
	// Removes object from correct interval. If interval comes empty, deletes it.
	// TIME: O(log N + 1) (amortized) [search + erase + insert]
	void erase(const T &element);
	
	// Searches for element. If it exists, returns true. Otherwise, returns false.
	// TIME: O(log N)
	bool find(const T &element) const;
protected:
	// Finds interval containing element. Returning value:
	// 1) element exists: (itetator to it, true)
	// 2) element does not exist, but left interval exists: (iterator to left interval, false)
	// 3) element does not exist and no correct interval exists: (this->end(), false)
	// TIME: O(log N)
	interval_set<T, Compare, Alloc>::iter_bool_pair interval_set::find_interval(const T &element);
	
	// Tries to merge 3 intervals: it and its both left and right neighbours.
	// If merging was successfully, return iterator to the new interval and true.
	// Otherwise, returns given iterator and false.
	// TIME: O(1) (amortized)
	inline iter_bool_pair interval_set<T, Compare, Alloc>::merge(interval_set<T, Compare, Alloc>::iterator it);
};

#include "interval_set.hpp"
#endif // INTERVAL_SET_H
