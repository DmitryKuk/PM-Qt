// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include <algorithm>

template<class Container, class Func>
void for_each(const Container &cont, const Func &f)
{
	std::for_each(begin(cont), end(cont), f);
}

template<class Container, class Func>
void for_each(Container &cont, const Func &f)
{
	std::for_each(begin(cont), end(cont), f);
}
