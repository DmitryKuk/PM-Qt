// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include "cryptokernel/cryptokernel.h"

cryptokernel k;

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
	if (typeName.isEmpty()) return invalid_type_id;
	
	type_id_t new_type_id = this->kernel_.add_type(const std::string &type_name);
	if (new_type_id == invalid_type_id) return invalid_type_id;
	
	this->types_[new_type_id] = Type();
	return new_type_id;
}

type_id_t setType(type_id_t tid, const QString &typeName)
{
	if (typeName.isEmpty()) return invalid_type_id;
	
	auto it = this->types_.find(tid);
	if (it == this->types_.end()) return invalid_type_id;
	
	// Ask CryptoKernel!!!
	it->second.name = typeName;
	return tid;
}


// Field management (field ids are unique for same type)
std::vector<tfield_id_t> CryptoKernelAgent::fields(type_id_t tid) const
{
	std::vector<tfield_id_t> res;
	auto it = this->types_.find(tid);
	if (it != this->types_.end()) {
		res.reserve(it->second.fields.size());
		for (auto &x: it->second.fields) res.push_back(x.first);
	}
	return res;
}

QString CryptoKernelAgent::field(type_id_t tid, tfield_id_t fid) const
{
	auto it1 = this->types_.find(tid);
	if (it1 != this->types_.end()) {
		auto it2 = it1->second.fields.find(fid);
		if (it2 != it1->second.fields.end())
			return it2->second;
	}
	return "";
}

tfield_id_t CryptoKernelAgent::addField(type_id_t tid, const QString &fieldName)
{
	if (typeName.isEmpty()) return invalid_type_id;
	
	auto it = this->types_.find(tid);
	if (it == this->types_.end()) return invalid_type_id;
	
	// Ask CryptoKernel!!!
	it->second.name = typeName;
	return tid;
}

tfield_id_t CryptoKernelAgent::setField(type_id_t tid, tfield_id_t fid, const QString &fieldName)
{
	
}
