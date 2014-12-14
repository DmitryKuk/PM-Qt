// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Hashers
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::hash1::operator()(const Key1 *k) const
{ return this->Hash1::operator()(*k); }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::hash2::operator()(const Key2 *k) const
{ return this->Hash2::operator()(*k); }


// Predictors
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
bool
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::pred1::operator()(const Key1 *k1, const Key1 *k2) const
{ return this->Pred1::operator()(*k1, *k2); }

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
bool
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::pred2::operator()(const Key2 *k1, const Key2 *k2) const
{ return this->Pred2::operator()(*k1, *k2); }


// Iterators
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::begin() const
{
	return this->data_.begin();
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::end() const
{
	return this->data_.end();
}


// Constructors
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::doublemap()											// Default
{}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::doublemap(const doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &other):	// Copy
	data_(other.data_),
	map1_(other.map1_),
	map2_(other.map2_)
{}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::doublemap(doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &&other):		// Move
	data_(std::move(other.data_)),
	map1_(std::move(other.map1_)),
	map2_(std::move(other.map2_))
{}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::pair<typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert(const triple<Key1, Key2, Value> &t)
{
	if (this->map1_.find(&t.first) != this->map1_.end()
		|| this->map2_.find(&t.second) != this->map2_.end())
		return std::move(std::make_pair(std::move(this->end()), false));
	
	this->data_.push_back(t);
	auto it = --(this->data_.end());
	
	this->map1_[&it->first] = it;
	this->map2_[&it->second] = it;
	return std::make_pair(std::move(it), true);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
std::pair<typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator, bool>
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::insert(triple<Key1, Key2, Value> &&t)
{
	if (this->map1_.find(&t.first) != this->map1_.end()
		|| this->map2_.find(&t.second) != this->map2_.end())
		return std::move(std::make_pair(std::move(this->end()), false));
	
	this->data_.push_back(std::move(t));
	auto it = --(this->data_.end());
	
	this->map1_[&it->first] = it;
	this->map2_[&it->second] = it;
	return std::make_pair(std::move(it), true);
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase(doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator it)
{
	if (it == this->end()) return;
	
	this->map1_.erase(&it->first);
	this->map2_.erase(&it->second);
	this->data_.erase(it);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
void
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase1(const Key1 &k1)
{
	this->erase(this->find1(k1));
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
void
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::erase2(const Key2 &k2)
{
	this->erase(this->find2(k2));
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
size_t
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::size() const
{
	// Not this->data_.size(), because it can be O(n).
	return this->map1_.size();
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::find1(const Key1 &k1) const
{
	auto it = this->map1_.find(&k1);
	if (it == this->map1_.end()) return this->end();
	return std::move(it->second);
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
typename doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::const_iterator
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::find2(const Key2 &k2) const
{
	auto it = this->map2_.find(&k2);
	if (it == this->map2_.end()) return this->end();
	return std::move(it->second);
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
Value &
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at1(const Key1 &k1)
{
	return this->map1_.at(&k1)->third;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
inline
const Value &
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at1(const Key1 &k1) const
{
	return this->map1_.at(&k1)->third;
}


template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
Value &
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at2(const Key2 &k2)
{
	return this->map2_.at(&k2)->third;
}

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
const Value &
doublemap<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2>::at2(const Key2 &k2) const
{
	return this->map2_.at(&k2)->third;
}
