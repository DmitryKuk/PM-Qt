// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H

#include <map>

template<class T>
class interval_set: private std::map<T, T>
{
public:
	typedef std::map<T, T> parent_container;
	
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
	inline interval_set();
	inline interval_set(const interval_set<T> &other);
	
	inline interval_set<T> & operator=(const interval_set<T> &other);
	
	// Inserts new interval or element as [element, element + 1) interval and merges it.
	// Correct hint speeds up searching of interval.
	inline std::pair<interval_set<T>::iterator, bool> insert(const T &element, interval_set<T>::iterator hint = this->end());
	inline std::pair<interval_set<T>::iterator, bool> insert(const std::pair<T, T> &interval, interval_set<T>::iterator hint = this->end());
	
	// Removes object from correct interval. If interval comes empty, deletes it.
	// Correct hint speeds up searching of interval.
	void erase(const T &element);
protected:
	// Tries to merge 3 intervals: it and its both left and right neighbours.
	// If merging was successfully, return iterator to the new interval and true.
	// Otherwise, returns given iterator and false.
	inline std::pair<interval_set<T>::iterator, bool> interval_set<T>::merge(interval_set<T>::iterator it);
};

#include "interval_set.h"
#endif // INTERVAL_SET_H
