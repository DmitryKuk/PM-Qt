// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains only simple scalar types and some constants.
// No classes, widgets, items and others!

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>		// For uint32_t
#include <limits>		// For std::numeric_limits
#include <functional>	// For std::hash
#include <iostream>		// For operators << and >>

namespace types {

// Universal id: assotiates any type with any identificator type (integral type)
template<class ReprType,				// Type (class), which the id class represents
		 class RawId,					// Type for raw operations (int, long, etc)
		 class MappingType = double>	// Type for calculating in mapping: [min1, max1] -> [min2, max2] (for generators)
class universal_id
{
public:
	// Types
	typedef ReprType repr_type;
	typedef RawId raw_id_type;
	typedef MappingType mapping_type;
	
	// Raw limits
	inline static constexpr raw_id_type raw_invalid() noexcept;
	inline static constexpr raw_id_type raw_min() noexcept;
	inline static constexpr raw_id_type raw_max() noexcept;
	
	// Limits
	inline static constexpr universal_id<ReprType, RawId, MappingType> invalid() noexcept;
	inline static constexpr universal_id<ReprType, RawId, MappingType> min() noexcept;
	inline static constexpr universal_id<ReprType, RawId, MappingType> max() noexcept;
	
	
	// Constructors
	explicit inline universal_id(raw_id_type value = universal_id<ReprType, RawId, MappingType>::raw_invalid()) noexcept;
	inline universal_id(const universal_id<ReprType, RawId, MappingType> &other) noexcept;
	
	// Assign operators
	inline universal_id<ReprType, RawId, MappingType> & operator=(raw_id_type value) noexcept;
	inline universal_id<ReprType, RawId, MappingType> & operator=(const universal_id<ReprType, RawId, MappingType> &other) noexcept;
	
	// Access and status functions
	inline raw_id_type value() const noexcept;
	inline bool is_valid() const noexcept;
	inline bool is_invalid() const noexcept;
	
	
	// Comparators
	inline bool operator<(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
	inline bool operator<=(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
	inline bool operator>(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
	inline bool operator>=(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
	inline bool operator==(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
	inline bool operator!=(const universal_id<ReprType, RawId, MappingType> &other) const noexcept;
private:
	raw_id_type value_;
};


typedef uint32_t raw_id_t;	// Raw type for all

// Types' ids
class	type_id_class_ {};
typedef raw_id_t type_id_raw_;
typedef universal_id<type_id_class_,	type_id_raw_>	type_id;	// Complete id

// Types' fields' ids
class	tfield_id_class_ {};
typedef raw_id_t tfield_id_raw_;
typedef universal_id<tfield_id_class_,	tfield_id_raw_>	tfield_id;	// Complete id

// Records' ids
class	record_id_class_ {};
typedef raw_id_t record_id_raw_;
typedef universal_id<record_id_class_,	record_id_raw_>	record_id;	// Complete id

// Records' fields' ids
class	rfield_id_class_ {};
typedef raw_id_t rfield_id_raw_;
typedef universal_id<rfield_id_class_,	rfield_id_raw_>	rfield_id;	// Complete id

// Groups' ids
class	group_id_class_ {};
typedef raw_id_t group_id_raw_;
typedef universal_id<group_id_class_,	group_id_raw_>	group_id;	// Complete id

};	// namespace types


// Input/output
template<class ReprType, class RawId, class MappingType>
inline std::istream & operator>>(std::istream &stream, types::universal_id<ReprType, RawId, MappingType> &id);

template<class ReprType, class RawId, class MappingType>
inline std::ostream & operator<<(std::ostream &stream, const types::universal_id<ReprType, RawId, MappingType> &id);


// Limits as std::numeric_limits
// WARGING! Addig numeric_limits<universal_id<ReprType, RawId, MappingType>> into namespace std!
namespace std {
template<class ReprType, class RawId, class MappingType>
class numeric_limits<types::universal_id<ReprType, RawId, MappingType>>:
	public numeric_limits<typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type>
{
public:
	inline static constexpr typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type min() noexcept;
	inline static constexpr typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type max() noexcept;
	inline static constexpr typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type lowest() noexcept;
};
};	// namespace std


// Hash as std::hash
// WARGING! Addig hash<universal_id<ReprType, RawId, MappingType>> into namespace std!
namespace std {
template<class ReprType, class RawId, class MappingType>
struct hash<types::universal_id<ReprType, RawId, MappingType>>
{
	inline size_t operator()(const types::universal_id<ReprType, RawId, MappingType> &id) const;
private:
	hash<typename types::universal_id<ReprType, RawId, MappingType>::raw_id_type> raw_hasher_;
};
};	// namespace std


#include "types.hpp"
#endif // TYPES_H
