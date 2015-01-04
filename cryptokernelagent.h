// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains cryptokernel agent. Agent must use Kernel for all kinds
// of cryptography tasks: generate new ids, encode or decode data...
// Agent is interface for GUI to do all cryptography actions.

// Example: [     GUI      ] <-> [          Agent          ] <-> [    Kernel    ]
//          (Qt/ncurses/...)     (depends on GUI and Kernel)     (lib/daemon/...)
// This version of program uses Qt GUI, so Agent has Qt-based API too. Agent may has
// own cache for some data. Some data caches in PasswordItem or GroupItem objects.

#ifndef CRYPTOKERNELAGENT_H
#define CRYPTOKERNELAGENT_H

#include <QObject>
#include <QString>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "cryptokernel/cryptokernel.h"
#include "mainwindow.h"

#include "abstractitem.h"
#include "groupitem.h"
#include "recorditem.h"
#include "typeitem.h"

class CryptoKernelAgent
{
public:
	CryptoKernelAgent();
	~CryptoKernelAgent();
	
	inline MainWindow * mainWindow() const;
	inline bool addMainWindow(MainWindow *mainWindow);
	inline bool removeMainWindow();
	
	void showData();
	void updateRecordListItems();
private:
	inline QString typeName(RecordItem *recordItem) const;
	inline QString parentGroupName(RecordItem *recordItem) const;
	
	inline void showRecordInList(RecordItem *recordItem);
	inline void hideRecordInList(RecordItem *recordItem);
	
	
	cryptokernel kernel_;		// Cryptokernel used by this agent
	MainWindow *mainWindow_;	// Main window attached to this agent
	
	
	// Groups maps
	// Group item * <-> group id
	std::unordered_map<GroupItem *, group_id_t> groupItemsMap_;
	std::unordered_map<group_id_t, GroupItem *> groupIdsMap_;
	// The root group in mainWindow->leftPanel->groupListWidget availible as
	// groupIdsMap_[kernel_.root_group_id()]
	
	
	// Records maps
	// Record item * -> (record id, record item in record list *)
	std::unordered_map<RecordItem *,
					   std::pair<record_id_t,
					   			 QTreeWidgetItem *>> recordItemsMap_;
	// Record id -> record item *
	std::unordered_map<record_id_t, RecordItem *> recordIdsMap_;
	
	
	// Records are shown in record list in mainWindow->mainWidget->recordListWidget
	// This set is part of set of all "QTreeWidgetItem *"s im recordItemsMap_
	std::unordered_set<QTreeWidgetItem *> shownRecordItems_;
	
	
	// Type maps and root group
	// Type item <-> type id
	std::unordered_map<TypeItem *, type_id_t> typeItemsMap_;
	std::unordered_map<type_id_t, TypeItem *> typeIdsMap_;
	GroupItem *rootTypeGroup_;	// Root group of types in mainWindow->leftPanel->groupListWidget
	
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent(const CryptoKernelAgent &other);
	
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent & operator=(const CryptoKernelAgent &other);
};

// Connects (or disconnects) agent and its main window
inline bool connect(CryptoKernelAgent *agent, MainWindow *mainWindow);
inline bool disconnect(CryptoKernelAgent *agent, MainWindow *mainWindow);

#include "cryptokernelagent.hpp"
#endif // CRYPTOKERNELAGENT_H
