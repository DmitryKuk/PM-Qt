// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <list>

class Password:
	public std::list<std::pair<id_t, std::string>>
{};

#endif // PASSWORD_H
