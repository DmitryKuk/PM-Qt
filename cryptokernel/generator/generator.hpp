// Author: Dmitry Kukovinets (d1021976@gmail.com)

template<class Num>
generator::simple<Num>::~simple()
{}


template<class Num>
inline
const generator::simple<Num> &
operator>>(const generator::simple<Num> &g, Num &n)
{
	n = g.generate();
	return g;
}
