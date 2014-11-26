// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

CryptoKernelAgent::CryptoKernelAgent()
{}

CryptoKernelAgent::~CryptoKernelAgent()
{}


// Type management (every type has own set of fields)
std::vector<type_id_t> CryptoKernelAgent::types() const
{
	std::vector<type_id_t> res;
	res.reserve(this->types_.size());
	for (auto &x: this->types_) res.push_back(x.first);
	return res;
}

QString CryptoKernelAgent::type(type_id_t tid) const
{
	auto it = this->types_.find(tid);
	if (it == this->types_.end()) return "";
	else return it->second.name;
}

type_id_t CryptoKernelAgent::addType(const QString &typeName)
{
	return 0;
}


// Field management (field ids are unique for same type)
std::vector<field_id_t> CryptoKernelAgent::fields(type_id_t tid) const
{
	std::vector<field_id_t> res;
	auto it = this->types_.find(tid);
	if (it != this->types_.end()) {
		res.reserve(it->second.fields.size());
		for (auto &x: it->second.fields) res.push_back(x.first);
	}
	return res;
}

QString CryptoKernelAgent::field(type_id_t tid, field_id_t fid) const
{
	auto it1 = this->types_.find(tid);
	if (it1 != this->types_.end()) {
		auto it2 = it1->second.fields.find(fid);
		if (it2 != it1->second.fields.end())
			return it2->second;
	}
	return "";
}

field_id_t CryptoKernelAgent::addField(type_id_t tid, const QString &fieldName)
{
	return 0;
}
