// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef FIELDSSTORAGE_H
#define FIELDSSTORAGE_H

#include <QString>

#include <map>

#include "types.h"
#include "cryptokernelagent.h"

class FieldsStorage: protected std::unordered_map<field_id_t, QString>
{
public:
	FieldsStorage(CryptoKernelAgent *agent);
	
	QString operator[](field_id_t id) const;	// Returns value if it exists or empty string
	
	
private:
	CryptoKernelAgent *agent_;
};

#endif // FIELDSSTORAGE_H
