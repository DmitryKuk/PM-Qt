// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TEMPLATE_MAGIC_H
#define TEMPLATE_MAGIC_H

#include <tuple>
#include <limits>

// TODO: See later struct num<...>.

namespace template_magic {

// Returns the Nth type of the Types... or void.
// Usage:
//	get<0, int, char, long>::type var;					// var is of type int
//	get<5, int, char, long>::type var;					// Error: var is of type void
template<size_t N, class ...Types>
struct get
{
	typedef void type;
};

template<size_t N, class T, class ...Types>
struct get<N, T, Types...>
{
	typedef typename get<N - 1, Types...>::type type;
};

template<class T, class ...Types>
struct get<0, T, Types...>
{
	typedef T type;
};


// Returns the first (the last) type of Types... or void.
// Usage:
//	front<int, char, double>::type var;	// var is of type int
//	back<int, char, double>::type var;	// var is of type double
//	back<>::type var;					// Error: var is of type void
template<class ...Types>
struct front
{
	typedef void type;
};

template<class T, class ...Types>
struct front<T, Types...>
{
	typedef T type;
};

template<class ...Types>
struct back
{
	typedef typename get<sizeof...(Types) - 1, Types...>::type type;
};

template<>
struct back<>
{
	typedef void type;
};



// Indexes list
template<size_t ...Indexes>
struct index_list
{
	// Holds the next index_list type (0..N+1)
	typedef index_list<Indexes..., sizeof ...(Indexes)> next;
};


template<size_t N>
struct index_list_holder
{
	// Holds the index list: 0..N
	typedef typename index_list_holder<N - 1>::list::next list;
};


template<>
struct index_list_holder<0>
{
	// Holds the empty index list
	typedef index_list<> list;
};



// Type node (for [forward] type list, see below)
template<class ...Types>
struct type_node
{
	typedef void type;
	typedef void next;
};

template<class T, class ...Other>
struct type_node<T, Other...>
{
	typedef T type;
	typedef type_node<Other...> next;
};



// Returns node or type of the Nth node forwarding this.
// If N == 0, works with the current node.
template<size_t N, class ...Types>	// Empty for all non-type_node classes
struct get_node
{
	typedef void node;
	typedef void type;
};

template<size_t N, class ...Types>
struct get_node<N, type_node<Types...>>
{
	typedef typename get_node<N - 1, typename type_node<Types...>::next>::node node;
	typedef typename node::type type;
};

template<size_t N, class T>
struct get_node<N, type_node<T>>
{
	typedef typename get_node<N - 1, typename type_node<T>::next>::node node;
	typedef typename node::type type;
};

template<class ...Types>
struct get_node<0, type_node<Types...>>
{
	typedef type_node<Types...> node;
	typedef typename node::type type;
};

template<class T>
struct get_node<0, type_node<T>>
{
	typedef type_node<T> node;
	typedef typename node::type type;
};



template<size_t N, class List>
struct skip
{
	typedef void list;
};

template<size_t N, class List, class ResultList>
struct simple_cut_append
{
	typedef void list;
};

template<size_t N, class List>
struct cut_first
{
	typedef void list;
};

template<size_t Begin, size_t End, class List>
struct cut
{
	typedef void list;
};


// Type list [forward].
template<class ...Types>
struct type_list
{
	typedef std::tuple<Types...> tuple;
	typedef type_list<Types...> current;
	enum { size = sizeof...(Types) };	// Size of list
	
	// The first and the last nodes. Use front::type (back::type) to get type
	typedef type_node<Types...> front;
	typedef typename get_node<sizeof...(Types) - 1, front>::node back;
	
	
	// Returns the Nth type of types this list contains or void.
	// Usage:
	//	type_list<int, char, long>::get<0>::type var;	// var is of type int
	//	type_list<int, char, long>::get<0>::node var;	// var is of type type_node<int, char, long>
	template<size_t N>
	struct get
	{
		typedef typename get_node<N, front>::node node;
		typedef typename node::type type;
	};
	
	template<class ...Other>
	struct append
	{
		typedef type_list<Types..., Other...> list;
	};
	
	template<class ...Other>
	struct append<type_list<Other...>>
	{
		typedef type_list<Types..., Other...> list;
	};
	
	template<size_t N>
	struct cut_first
	{
		typedef typename template_magic::cut_first<N, current>::list list;
	};
	
	template<size_t Begin, size_t End>
	struct cut
	{
		typedef typename template_magic::skip<Begin, typename cut_first<End>::list>::list list;
	};
};

template<>
struct type_list<>
{
	typedef std::tuple<> tuple;
	typedef type_list<> current;
	enum { size = 0 };	// Size of list
	
	// The first and the last nodes. Use front::type (back::type) to get type
	typedef type_node<> front;
	typedef front back;
	
	
	// Returns the Nth type of types this list contains or void.
	// Usage:
	//	type_list<int, char, long>::get<0>::type var;	// var is of type int
	//	type_list<int, char, long>::get<0>::node var;	// var is of type type_node<int, char, long>
	template<size_t N>
	struct get
	{
		typedef typename get_node<N, front>::node node;
		typedef typename node::type type;
	};
	
	template<class ...Other>
	struct append
	{
		typedef type_list<Other...> list;
	};
	
	template<class ...Other>
	struct append<type_list<Other...>>
	{
		typedef type_list<Other...> list;
	};
	
	template<size_t Begin, size_t End>
	struct copy
	{
		typedef type_list<> list;
	};
	
	template<size_t N>
	struct cut_first
	{
		typedef type_list<> list;
	};
	
	template<size_t Begin, size_t End>
	struct cut
	{
		typedef type_list<> list;
	};
};


template<size_t N, class T, class ...Types>
struct skip<N, type_list<T, Types...>>
{
	typedef typename skip<N - 1, type_list<Types...>>::list list;
};

template<class T, class ...Types>
struct skip<0, type_list<T, Types...>>
{
	typedef type_list<T, Types...> list;
};

template<size_t N>
struct skip<N, type_list<>>
{
	typedef type_list<> list;
};


template<size_t N, class T, class ...Types, class ...ResultTypes>
struct simple_cut_append<N, type_list<T, Types...>, type_list<ResultTypes...>>
{
	typedef typename simple_cut_append<N - 1, type_list<Types...>, type_list<ResultTypes..., T>>::list list;
};

template<class T, class ...Types, class ...ResultTypes>
struct simple_cut_append<0, type_list<T, Types...>, type_list<ResultTypes...>>
{
	typedef type_list<ResultTypes...> list;
};

template<size_t N, class ...ResultTypes>
struct simple_cut_append<N, type_list<>, type_list<ResultTypes...>>
{
	typedef type_list<ResultTypes...> list;
};


template<size_t N, class ...Types>
struct cut_first<N, type_list<Types...>>
{
	typedef typename simple_cut_append<N, type_list<Types...>, type_list<>>::list list;
};


template<size_t Begin, size_t End, class ...Types>
struct cut<Begin, End, type_list<Types...>>
{
	typedef typename type_list<Types...>::template cut<Begin, End>::list list;
};



template<class NumType>
struct num
{
	typedef NumType type;
	
	template<size_t N, NumType Num, NumType ...Nums>
	struct get_
	{
		enum { value = num<NumType>::get_<N - 1, Nums...>::value };
	};

	template<NumType Num, NumType ...Nums>
	struct get_<0, Num, Nums...>
	{
		enum { value = Num };
	};
	
	template<size_t N, NumType Num, NumType ...Nums>
	struct get
	{
		static_assert(N < sizeof...(Nums), "Index out of range.");
		
		enum { value = num<NumType>::get_<N - 1, Nums...>::value };
	};
	
	
	template<NumType ...Nums>
	struct list_node
	{
		typedef void next;
		enum { value = 0 };
	};
	
	template<NumType Num, NumType ...Nums>
	struct list_node<Num, Nums...>
	{
		typedef typename num<NumType>::template list_node<Nums...> next;
		enum { value = Num };
	};
	
	template<NumType ...Nums>
	struct list
	{
		typedef typename num<NumType>::template list_node<Nums...> front;
	};
	
	
	static constexpr NumType min(NumType a, NumType b)
	{ return (b < a)? b: a; }
	
	template<NumType ...Nums>
	struct extract_min
	{
		enum { value = std::numeric_limits<NumType>::max };
	};
	
	template<NumType Num, NumType ...Nums>
	struct extract_min<Num, Nums...>
	{
		enum { value = num<NumType>::min(Num, num<NumType>::template extract_min<Nums...>::value) };
	};
	
	
	// template<NumType ...Nums>
	// struct sort
	// {
		
	// };
};



// For_each
// For_each for iterators
template<class InputIterator, class Func>
inline Func for_each(InputIterator first, InputIterator last, Func f);

template<class InputIterator, class Func>
inline Func for_each_iterator(InputIterator first, InputIterator last, Func f);


// For_each for containers
template<class Container, class Func>
inline Func for_each(Container &cont, Func f);

template<class Container, class Func>
inline Func for_each_iterator(Container &cont, Func f);


// For_each for tuples.
// NOTE: Func must have an operator(), that can take object of any type of tuple elements.
template<size_t I = 0, class ...Types, class Func>
Func for_each(std::tuple<Types...> &t, Func f);

template<size_t I = 0, class ...Types, class Func>
Func for_each(const std::tuple<Types...> &t, Func f);

};	// namespace template_magic

#include "template_magic.hpp"
#endif // TEMPLATE_MAGIC_H
