// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Iterators
template<class T>
inline iterator interval_set<T>::begin()
{
	return this->parent_container::begin();
}

template<class T>
inline const_iterator interval_set<T>::begin() const
{
	return this->parent_container::begin();
}

template<class T>
inline iterator interval_set<T>::end()
{
	return this->parent_container::end();
}

template<class T>
inline const_iterator interval_set<T>::end() const
{
	return this->parent_container::end();
}


// Constructors and operator=()
template<class T>
inline
interval_set<T>::::interval_set()
{}

template<class T>
inline
interval_set<T>::interval_set::interval_set(const interval_set<T> &other):
	parent_container(other)
{}

template<class T>
inline
interval_set<T> &
interval_set<T>::operator=(const interval_set<T> &other)
{
	this->parent_container::operator=(other);
	return *this;
}


// Inserts new interval into correct interval. If interval does not exists, creates it.
// Correct hint speeds up searching of interval.
template<class T>
inline
std::pair<interval_set<T>::iterator, bool>
interval_set::insert(const T &element, interval_set<T>::iterator hint)
{
	auto p = std::make_pair(element, element);	// p = [element, element]
	++p.second;									// p = [element, element + 1)
	return this->insert(p, hint);
}

template<class T>
inline
std::pair<interval_set<T>::iterator, bool>
interval_set::insert(const std::pair<T, T> &interval, interval_set<T>::iterator hint)
{
	
}


// Removes object from correct interval. If interval comes empty, deletes it.
// Correct hint speeds up searching of interval.
template<class T>
void
interval_set::erase(const T &element)
{
	
	
	// Forking interval: [left, right) => [left, element), [element + 1, right)
	T left = it->first;
	it->first = element; ++it->first;
	this->parent_container::insert(it, std::make_pair(left, element));
}


// Tries to merge 3 interval_set: it and its both left and right neighbours.
// If merging was successfully, return iterator to the new interval and true.
// Otherwise, returns given iterator and false.
template<class T>
inline
std::pair<interval_set<T>::iterator, bool>
interval_set<T>::merge(interval_set<T>::iterator it)
{
	// Helper lambda-function. Merges 2 intervals into one. If it is possible,
	// left is the result, returns true. Otherwise, returns false.
	static auto merge_intervals =
		[] (std::pair<T, T> &left, const std::pair<T, T> &right)
			-> bool
		{
			if (left.second < right.first || right.second < left.first) return false;
			if (right.first < left.first) left.first = right.first;
			if (left.second < right.second) left.second = right.second;
			return true;
		};
	
	// Helper lambda-function. Merges 2 intervals into one: given left and its right neighbour.
	// If merging completed successfully, returns iterator to inserted interval and true.
	// Otherwise, returns given iterator and false.
	// NOTE: Does NOT check iterators for equal to end().
	static auto merge_and_insert =
		[this] (interval_set<T>::iterator left)
			-> std::pair<interval_set<T>::iterator, bool>
		{
			auto right = left; ++right;
			std::pair<T, T> p = *left;
			if (merge_intervals(p, *right)) {
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
		res = merge_and_insert(--it);
		it = res.first;
	}
	
	auto right = it; ++right;
	if (it != end && right != end) {	// Merging with right neighbour
		auto res1 = merge_and_insert(it);
		if (res1.second) {
			res.first = res1.first;
			res.second = true;
		}
	}
	return res;
}
