// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains cryptokernel agent. Agent must use Kernel for all kinds
// of cryptography tasks: generate new ids, encode or decode data...
// Agent is interface for GUI to do all cryptography actions.

// Example: [     GUI      ] <-> [          Agent          ] <-> [    Kernel    ]
//          (Qt/ncurses/...)     (depends on GUI and Kernel)     (lib/daemon/...)
// This version of program uses Qt GUI, so Agent has Qt API too. Agent has own
// cache for some data. Some data caches in PasswordItem or GroupItem objects.

#ifndef CRYPTOKERNELAGENT_H
#define CRYPTOKERNELAGENT_H

#include <QString>

#include <vector>
#include <unordered_map>

#include "types.h"

// TODO: IMPLEMENT CORRECT addType() and AddField()!!!

class CryptoKernelAgent
{
public:
	CryptoKernelAgent();
	~CryptoKernelAgent();
	
	// Type management (every type has own set of fields)
	std::vector<type_id_t> types() const;			// Returns all types ids
	QString type(type_id_t tid) const;				// Returns type id if it exists or empty string
	type_id_t addType(const QString &typeName);		// Generates new type id and adds it with name
	type_id_t setType(type_id_t tid, const QString &typeName);	// Sets new name for existing type
																// or returns invalid_type_id
	
	// Field management (field ids are unique for same type)
	std::vector<field_id_t> fields(type_id_t tid) const;			// Returns all fields ids for type or empty vector
	QString field(type_id_t tid, field_id_t fid) const;				// Returns field id if it exists or empty string
	field_id_t addField(type_id_t tid, const QString &fieldName);	// Generates new field id and adds it with name
	field_id_t setField(type_id_t tid, field_id_t fid, const QString &fieldName);	// Sets new name for existing
																					// field of existing type or
																					// returns invalid_field_id
protected:
	type_id_t addRandomTypeId();	// Adds a new type with empty name and fields or returns invalid_type_id
	field_id_t addRandomFieldId(type_id_t tid);	// Adds a new field for existing type or returns invalid_field_id
private:
	struct Type
	{
		QString name;
		std::unordered_map<field_id_t, QString> fields;
	};
	
	std::unordered_map<type_id_t, Type> types_;	// Types with their names and fields (id + name)
	
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent(const CryptoKernelAgent &other);
	
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent & operator=(const CryptoKernelAgent &other);
};

#endif // CRYPTOKERNELAGENT_H
