// Author: Dmitry Kukovinets (d1021976@gmail.com)

// Raw limits
template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
types::universal_id<ReprType, RawId, MappingType>::raw_invalid() noexcept
{ return ::std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>::raw_id_type>::max(); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
types::universal_id<ReprType, RawId, MappingType>::raw_min() noexcept
{ return ::std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>::raw_id_type>::min(); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
types::universal_id<ReprType, RawId, MappingType>::raw_max() noexcept
{ return ::std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>::raw_id_type>::max() - 1; }


// Limits
template<class ReprType, class RawId, class MappingType>
inline constexpr
types::universal_id<ReprType, RawId, MappingType>
types::universal_id<ReprType, RawId, MappingType>::invalid() noexcept
{ return types::universal_id<ReprType, RawId, MappingType>(types::universal_id<ReprType, RawId, MappingType>::raw_invalid()); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
types::universal_id<ReprType, RawId, MappingType>
types::universal_id<ReprType, RawId, MappingType>::min() noexcept
{ return types::universal_id<ReprType, RawId, MappingType>(types::universal_id<ReprType, RawId, MappingType>::raw_min()); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
types::universal_id<ReprType, RawId, MappingType>
types::universal_id<ReprType, RawId, MappingType>::max() noexcept
{ return types::universal_id<ReprType, RawId, MappingType>(types::universal_id<ReprType, RawId, MappingType>::raw_max()); }


// Constructors
template<class ReprType, class RawId, class MappingType>
inline
types::universal_id<ReprType, RawId, MappingType>::universal_id(types::universal_id<ReprType, RawId, MappingType>::raw_id_type value) noexcept:
	value_(value)
{}

template<class ReprType, class RawId, class MappingType>
inline
types::universal_id<ReprType, RawId, MappingType>::universal_id(const types::universal_id<ReprType, RawId, MappingType> &other) noexcept:
	value_(other.value())
{}


// Assign operators
template<class ReprType, class RawId, class MappingType>
inline
types::universal_id<ReprType, RawId, MappingType> &
types::universal_id<ReprType, RawId, MappingType>::operator=(types::universal_id<ReprType, RawId, MappingType>::raw_id_type value) noexcept
{
	this->value_ = value;
	return *this;
}

template<class ReprType, class RawId, class MappingType>
inline
types::universal_id<ReprType, RawId, MappingType> &
types::universal_id<ReprType, RawId, MappingType>::operator=(const types::universal_id<ReprType, RawId, MappingType> &other) noexcept
{
	this->value_ = other.value();
	return *this;
}


// Access and status functions
template<class ReprType, class RawId, class MappingType>
inline
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
types::universal_id<ReprType, RawId, MappingType>::value() const noexcept
{ return this->value_; }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::is_valid() const noexcept
{ return !this->is_invalid(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::is_invalid() const noexcept
{ return *this == types::universal_id<ReprType, RawId, MappingType>::invalid(); }


// Comparators
template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator<(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() < other.value(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator<=(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() <= other.value(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator>(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() > other.value(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator>=(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() >= other.value(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator==(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() == other.value(); }

template<class ReprType, class RawId, class MappingType>
inline
bool
types::universal_id<ReprType, RawId, MappingType>::operator!=(const types::universal_id<ReprType, RawId, MappingType> &other) const noexcept
{ return this->value() != other.value(); }


// Input/output
template<class ReprType, class RawId, class MappingType>
inline
std::istream &
operator>>(std::istream &stream, types::universal_id<ReprType, RawId, MappingType> &id)
{
	typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type raw_id;
	stream >> raw_id;
	id = raw_id;
	return stream;
}

template<class ReprType, class RawId, class MappingType>
inline
std::ostream &
operator<<(std::ostream &stream, const types::universal_id<ReprType, RawId, MappingType> &id)
{ return stream << id.value(); }


// Limits as std::numeric_limits
template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>>::min() noexcept
{ return types::universal_id<ReprType, RawId, MappingType>::raw_min(); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>>::max() noexcept
{ return types::universal_id<ReprType, RawId, MappingType>::raw_max(); }

template<class ReprType, class RawId, class MappingType>
inline constexpr
typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type
std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>>::lowest() noexcept
{ return std::numeric_limits<types::universal_id<ReprType, RawId, MappingType>>::raw_min(); }


// Hash as std::hash
template<class ReprType, class RawId, class MappingType>
inline
size_t
std::hash<types::universal_id<ReprType, RawId, MappingType>>::operator()(const types::universal_id<ReprType, RawId, MappingType> &id) const
{ return this->raw_hasher_(id.value()); }
