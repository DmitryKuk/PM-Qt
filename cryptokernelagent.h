// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef CRYPTOKERNELAGENT_H
#define CRYPTOKERNELAGENT_H

#include <string>
#include <map>

#include "types.h"
#include "passwordtree.h"

class CryptoKernelAgent
{
public:
	typedef std::unordered_map<id_t, id_t> map;
	
	CryptoKernelAgent(const std::string filename);
	
	map groups() const;
	map items() const;
	
	std::string data(const id_t &id) const;
};

#endif // CRYPTOKERNELAGENT_H
