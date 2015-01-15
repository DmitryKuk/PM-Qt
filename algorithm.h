// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ALGORITHM_H
#define ALGORITHM_H

template<class Container, class Func>
void for_each(const Container &cont, const Func &f);

template<class Container, class Func>
void for_each(Container &cont, const Func &f);


#include "algorithm.hpp"
#endif // ALGORITHM_H
