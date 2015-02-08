// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline bool CryptoKernelAgent::Groups::cached(types::group_id id)
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


inline bool CryptoKernelAgent::Records::cached(types::record_id id)
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


inline bool CryptoKernelAgent::Types::cached(types::type_id id)
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
