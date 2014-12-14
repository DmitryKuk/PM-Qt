// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TRIPLE_H
#define TRIPLE_H

template<class T1, class T2, class T3>
struct triple
{
	T1 first;
	T2 second;
	T3 third;
	
	inline triple();
	inline triple(const T1 &first, const T2 &second, const T3 &third);
	inline triple(T1 &&first, T2 &&second, T3 &&third);
};


template<class T1, class T2, class T3>
triple<T1, T2, T3> && make_triple(const T1 &first, const T2 &second, const T3 &third);

template<class T1, class T2, class T3>
triple<T1, T2, T3> && make_triple(T1 &&first, T2 &&second, T3 &&third);

#include "triple.hpp"
#endif // TRIPLE_H
