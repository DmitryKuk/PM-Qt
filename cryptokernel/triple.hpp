// Author: Dmitry Kukovinets (d1021976@gmail.com)

template<class T1, class T2, class T3>
inline
triple<T1, T2, T3>::triple()
{}

template<class T1, class T2, class T3>
inline
triple<T1, T2, T3>::triple(const T1 &first, const T2 &second, const T3 &third):
	first(first), second(second), third(third)
{}

template<class T1, class T2, class T3>
inline
triple<T1, T2, T3>::triple(T1 &&first, T2 &&second, T3 &&third):
	first(std::move(first)), second(std::move(second)), third(std::move(third))
{}


template<class T1, class T2, class T3>
triple<T1, T2, T3>
make_triple(const T1 &first, const T2 &second, const T3 &third)
{
	return triple<T1, T2, T3>(first, second, third);
}
