// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Iterators
template<class T, class Compare, class Alloc>
inline iterator interval_set<T, Compare, Alloc>::begin()
{
	return this->parent_container::begin();
}

template<class T, class Compare, class Alloc>
inline const_iterator interval_set<T, Compare, Alloc>::begin() const
{
	return this->parent_container::begin();
}

template<class T, class Compare, class Alloc>
inline iterator interval_set<T, Compare, Alloc>::end()
{
	return this->parent_container::end();
}

template<class T, class Compare, class Alloc>
inline const_iterator interval_set<T, Compare, Alloc>::end() const
{
	return this->parent_container::end();
}


// Constructors and operator=()
template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::::interval_set(const Compare &key_comp, const Alloc &alloc):
	parent_container(key_comp, alloc)
{}

template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::interval_set::interval_set(const interval_set<T, Compare, Alloc> &other):
	parent_container(other)
{}

template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc> &
interval_set<T, Compare, Alloc>::operator=(const interval_set<T, Compare, Alloc> &other)
{
	this->parent_container::operator=(other);
	return *this;
}


// Inserts new interval or element as [element, element + 1) interval and merges it.
// Correct hint speeds up searching of interval.
template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::iter_bool_pair
interval_set::insert(const T &element,
					 interval_set<T, Compare, Alloc>::iterator hint)
{
	auto p = std::make_pair(element, element);	// p = [element, element]
	++p.second;									// p = [element, element + 1)
	return this->insert(p, hint);
}

template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::iter_bool_pair
interval_set::insert(const std::pair<T, T> &interval,
					 interval_set<T, Compare, Alloc>::iterator hint)
{
	
}


template<class T, class Compare, class Alloc>
interval_set<T, Compare, Alloc>::iter_bool_pair
interval_set::find_interval(const T &element)
{
	Compare cmp = this->key_comp();	// Comparator
	auto it = this->lower_bound(element);	// Searching for correct interval
	// element exists if: (it->first <= element && element < it->second) == true
	// If element >= it->second, returns false: it (or ++it) is the nearest interval.
	if (it != this->end() && !cmp(element, it->first)) {
		if (cmp(element, it->second)) return std::make_pair(std::move(it), true);
		return std::make_pair(std::move(it), false);
	}
	return std::make_pair(this->end(), false);	// element does not exist
}


// Removes object from correct interval. If interval comes empty, deletes it.
template<class T, class Compare, class Alloc>
void
interval_set::erase(const T &element)
{
	Compare cmp = this->key_comp();	// Comparator
	// Helper lambda-function. Returns true, if a equals to b, false otherwise.
	static auto are_equal =
		[&cmp] (const T &a,
				const T &b)
		-> bool
		{
			if (cmp(a, b) || cmp(b, a)) return false;
			return true;
		};
	
	auto p = this->find_interval(element);
	if (!p.second) return;	// element does not exist
	
	// Forking interval: [l1, r1) => [l1, element), [l2, r1); l2 = element + 1
	T left = it->first;
	it->first = element; ++(it->first);
	
	this->parent_container::insert(it, std::make_pair(left, element));
}


// Tries to merge 3 interval_set: it and its both left and right neighbours.
// If merging was successfully, return iterator to the new interval and true.
// Otherwise, returns given iterator and false.
template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::iter_bool_pair
interval_set<T, Compare, Alloc>::merge(interval_set<T, Compare, Alloc>::iterator it)
{
	// Helper lambda-function. Merges 2 intervals into one. If it is possible,
	// left is the result, returns true. Otherwise, returns false.
	static auto merge_intervals =
		[] (std::pair<T, T> &left,
			const std::pair<T, T> &right,
			const Compare &cmp)
		-> bool
		{
			if (cmp(left.second, right.first) || cmp(right.second, left.first)) return false;
			if (cmp(right.first, left.first))	left.first	= right.first;
			if (cmp(left.second, right.second))	left.second	= right.second;
			return true;
		};
	
	// Helper lambda-function. Merges 2 intervals into one: given left and its right neighbour.
	// If merging completed successfully, returns iterator to inserted interval and true.
	// Otherwise, returns given iterator and false.
	// NOTE: Does NOT check iterators for equal to end().
	static auto merge_with_right =
		[this] (interval_set<T, Compare, Alloc>::iterator left)
interval_set<T, Compare, Alloc>::iter_bool_pair
		{
			auto right = left; ++right;
			std::pair<T, T> p = *left;
			if (merge_intervals(p, *right, this->key_comp())) {
				auto tmp = right; ++tmp;
				this->parent_container::erase(left);	// Cleaning
				this->parent_container::erase(right);
				return std::make_pair(this->parent_container::insert(tmp, p), true);	// Inserting
			}
			return std::make_pair(left, false);
		};
	
	auto res = std::make_pair(it, false);
	auto end = this->end();
	if (it == end) return res;
	
	if (it != this->begin()) {	// Merging with left neighbour
		res = merge_with_right(--it);
		it = res.first;
	}
	
	auto right = it; ++right;
	if (it != end && right != end) {	// Merging with right neighbour
		auto res1 = merge_with_right(it);
		if (res1.second) {
			res.first = res1.first;
			res.second = true;
		}
	}
	return res;
}
