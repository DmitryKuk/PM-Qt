// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>

#include "types.h"
#include "double_map.h"

namespace io {

typedef std::exception exception;

// NOTE: ALL these fuctions throw io::exception exceptions!
// It means that operation failed. Don't look at what()-string,
// manually check function parameters.


// Number
template<class Id>
inline void read(std::istream &stream, Id &id);

template<class Id>
inline void write(std::ostream &stream, const Id &id);


// Id
template<class ReprType, class RawId, class ...Args>
inline void read(std::istream &stream, types::universal_id<ReprType, RawId, Args...> &id);

template<class ReprType, class RawId, class ...Args>
inline void write(std::ostream &stream, const types::universal_id<ReprType, RawId, Args...> &id);


// String
template<>
inline void read(std::istream &stream, std::string &str);

template<>
inline void write(std::ostream &stream, const std::string &str);

inline void read_raw_string(std::istream &stream, std::string &str, size_t len);
inline void write_raw_string(std::ostream &stream, const std::string &str);


// Double_map
template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void read(std::istream &stream, double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &dmap);

template<class Key1, class Key2, class Value, class Hash1, class Hash2, class Pred1, class Pred2>
void write(std::ostream &stream, const double_map<Key1, Key2, Value, Hash1, Hash2, Pred1, Pred2> &dmap);

};	// namespace io

#include "io.hpp"
#endif // IO_H
