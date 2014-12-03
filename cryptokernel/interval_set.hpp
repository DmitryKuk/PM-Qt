// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Iterators
template<class T, class Compare, class Alloc>
inline typename interval_set<T, Compare, Alloc>::const_iterator interval_set<T, Compare, Alloc>::begin() const
{
	return this->parent_container::begin();
}

template<class T, class Compare, class Alloc>
inline typename interval_set<T, Compare, Alloc>::const_iterator interval_set<T, Compare, Alloc>::end() const
{
	return this->parent_container::end();
}


// Constructors and operator=()
template<class T, class Compare, class Alloc>
inline
interval_set<T, Compare, Alloc>::interval_set(const Compare &key_comp, const Alloc &alloc):
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
// TIME: O(log N + 1) (amortized) [search + insert + merge]
template<class T, class Compare, class Alloc>
inline
void
interval_set<T, Compare, Alloc>::insert(const T &element)
{
	auto p = std::make_pair(element, element);	// p = [element, element]
	++p.second;									// p = [element, element + 1)
	return this->insert(p);
}

template<class T, class Compare, class Alloc>
void
interval_set<T, Compare, Alloc>::insert(const std::pair<T, T> &interval)
{
	Compare cmp = this->key_comp();	// Comparator
	// Helper lambda-function. Returns true, if a equals to b, false otherwise.
	static auto are_equal =
		[&cmp] (const T &a, const T &b) -> bool
		{
			return (cmp(a, b) || cmp(b, a))? false: true;
		};
	
	if (!cmp(interval.first, interval.second)) return;	// Checking interval
	
	auto p = this->find_interval(interval.first);
	
	if (p.second &&
		are_equal(p.first->first, interval.first) &&
		cmp(p.first->second, interval.second))	// Stretch founded interval to new size
		p.first->second = interval.second;	// [5, 15) + [5, 20) => [5, 20)
	else {	// Insert new interval
		if (p.first != this->end()) ++p.first;	// p.first points to next interval
		auto it = this->parent_container::insert(p.first, interval);
		this->merge(it);	// Merge inserted interval with its neighbours
	}
}


// Removes object from correct interval. If interval comes empty, deletes it.
// TIME: O(log N + 1) (amortized) [search + erase + insert]
template<class T, class Compare, class Alloc>
void
interval_set<T, Compare, Alloc>::erase(const T &element)
{
	Compare cmp = this->key_comp();	// Comparator
	// Helper lambda-function. Returns true, if a equals to b, false otherwise.
	static auto are_equal =
		[&cmp] (const T &a, const T &b) -> bool
		{
			return (cmp(a, b) || cmp(b, a))? false: true;
		};
	
	auto p = this->find_interval(element);
	if (!p.second) return;	// element does not exist
	
	// Forking interval: [left, right) => [left, element), [element + 1, right)
	if (are_equal(element, p.first->first)) {	// [left = element, right) => [element + 1, right)
		std::pair<T, T> interval = std::move(*p.first); ++interval.first;	// left => element + 1
		p.first = this->parent_container::erase(p.first);	// Now p.first points to next interval
		if (cmp(interval.first, interval.second))	// Check: interval is correct (non-zero size)
			this->parent_container::insert(p.first, std::move(interval));
	} else {
		T element1 = element; ++element1;
		if (are_equal(element1, p.first->second))	// [left, right) => [left, right - 1)
			p.first->second = element;
		else {	// [left, right) => [left, element), [element + 1, right)
			auto it1 = p.first; ++it1;
			this->parent_container::emplace_hint(it1, std::move(element1), std::move(p.first->second));
			p.first->second = element;
		}
	}
}


// Searches for element. If it exists, returns true. Otherwise, returns false.
// TIME: O(log N)
template<class T, class Compare, class Alloc>
bool
interval_set<T, Compare, Alloc>::find(const T &element) const
{
	Compare cmp = this->key_comp();	// Comparator
	auto it = this->lower_bound(element);	// Searching for correct interval
	if (it == this->end() || !cmp(element, it->second))
		return false;	// interval or element does not exist
	return true;
}


// Finds interval containing element. Returning value:
// 1) element exists: (itetator to it, true)
// 2) element does not exist, but left interval exists: (iterator to left interval, false)
// 3) element does not exist and no correct interval exists: (this->end(), false)
// TIME: O(log N)
template<class T, class Compare, class Alloc>
typename interval_set<T, Compare, Alloc>::iter_bool_pair
interval_set<T, Compare, Alloc>::find_interval(const T &element)
{
	Compare cmp = this->key_comp();	// Comparator
	auto it = this->lower_bound(element);	// Searching for correct interval
	// element exists if: (it->first <= element && element < it->second) == true
	// If element >= it->second, returns false: it (or ++it) is the nearest interval.
	if (it != this->end()) {
		if (cmp(element, it->second)) return std::make_pair(std::move(it), true);
		return std::make_pair(std::move(it), false);
	}
	return std::make_pair(std::move(this->end()), false);	// element does not exist
}


// Tries to merge 3 interval_set: it and its both left and right neighbours.
// If merging was successfully, return iterator to the new interval and true.
// Otherwise, returns given iterator and false.
// TIME: O(1) (amortized)
template<class T, class Compare, class Alloc>
inline
typename interval_set<T, Compare, Alloc>::iter_bool_pair
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
		-> interval_set<T, Compare, Alloc>::iter_bool_pair
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


// Iterators
template<class T, class Compare, class Alloc>
inline typename interval_set<T, Compare, Alloc>::iterator interval_set<T, Compare, Alloc>::begin()
{
	return this->parent_container::begin();
}

template<class T, class Compare, class Alloc>
inline typename interval_set<T, Compare, Alloc>::iterator interval_set<T, Compare, Alloc>::end()
{
	return this->parent_container::end();
}
