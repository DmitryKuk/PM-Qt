// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline bool CryptoKernelAgent::Groups::cached(group_id_t id)
{
	if (this->idsMap.find(id) == this->idsMap.end())
		return false;
	return true;
}

inline bool CryptoKernelAgent::Groups::cached(GroupItem *item)
{
	if (this->itemsMap.find(item) == this->itemsMap.end())
		return false;
	return true;
}


inline bool CryptoKernelAgent::Records::cached(record_id_t id)
{
	if (this->idsMap.find(id) == this->idsMap.end())
		return false;
	return true;
}

inline bool CryptoKernelAgent::Records::cached(RecordItem *item)
{
	if (this->itemsMap.find(item) == this->itemsMap.end())
		return false;
	return true;
}


inline bool CryptoKernelAgent::Types::cached(type_id_t id)
{
	if (this->idsMap.find(id) == this->idsMap.end())
		return false;
	return true;
}

inline bool CryptoKernelAgent::Types::cached(TypeItem *item)
{
	if (this->itemsMap.find(item) == this->itemsMap.end())
		return false;
	return true;
}
