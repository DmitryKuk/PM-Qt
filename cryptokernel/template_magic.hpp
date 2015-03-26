// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <type_traits>

// For_each for iterators
template<class InputIterator, class Func>
inline Func template_magic::for_each(InputIterator first, InputIterator last, Func f)
{ return std::move(std::for_each(std::move(first), std::move(last), std::move(f))); }

template<class InputIterator, class Func>
inline Func template_magic::for_each_iterator(InputIterator first, InputIterator last, Func f)
{
	for (; first != last; ++first) f(first);
	return std::move(f);
}


// For_each for containers
template<class Container, class Func>
inline Func template_magic::for_each(Container &cont, Func f)
{
	using namespace std;
	using namespace template_magic;
	
	return std::move(template_magic::for_each(begin(cont), end(cont), std::move(f)));
}

template<class Container, class Func>
inline Func template_magic::for_each_iterator(Container &cont, Func f)
{
	using namespace std;
	using namespace template_magic;
	
	return std::move(template_magic::for_each_iterator(begin(cont), end(cont), std::move(f)));
}


// Helpers for for_each for tuples.
namespace {

template<size_t I>
struct for_each_struct_: public for_each_struct_<I - 1>
{
	template<class Func, class ...Types>
	for_each_struct_(std::tuple<Types...> &t, Func &f):
		for_each_struct_<I - 1>(t, f)
	{ f(std::get<I - 1>(t)); }
	
	template<class Func, class ...Types>
	for_each_struct_(const std::tuple<Types...> &t, Func &f):
		for_each_struct_<I - 1>(t, f)
	{ f(std::get<I - 1>(t)); }
};

template<>
struct for_each_struct_<0>
{
	template<class Func, class ...Types>
	for_each_struct_(const std::tuple<Types...> &, const Func &)
	{}
};

};	// namespace


// For_each for tuples.
template<size_t I, class ...Types, class Func>
Func template_magic::for_each(std::tuple<Types...> &t, Func f)
{
	::for_each_struct_<std::tuple_size<std::tuple<Types...>>::value>(t, f);
	return std::move(f);
}

template<size_t I, class ...Types, class Func>
Func template_magic::for_each(const std::tuple<Types...> &t, Func f)
{
	::for_each_struct_<std::tuple_size<std::tuple<Types...>>::value>(t, f);
	return std::move(f);
}
