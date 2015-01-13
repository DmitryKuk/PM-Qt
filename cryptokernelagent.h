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
	
	MainWindow * mainWindow() const;
	
	void run();
	
	// Slots
	// Update functions
	void updateRecordListItems();
	void updateRecordContent();
	
	// Record content
	void onNameClicked();
	void onNameChanged(QString newName);
	
	void onTypeClicked();
	void onTypeChanged(QString newTypeName);
	
	void onGroupClicked();
	void onGroupChanged(QString newGroupName);
	
	void onFieldClicked(int index);
	void onFieldChanged(int index, QString newText);
	
	// Add group/record/type, remove selected
	void addGroup();
	void addRecord();
	void addType();
	void remove();
private:
	void showData();
	
	QString typeName(RecordItem *recordItem) const;
	QString parentGroupName(RecordItem *recordItem) const;
	
	void showRecordInList(RecordItem *recordItem);
	void hideRecordInList(RecordItem *recordItem);
	
	
	cryptokernel kernel_;		// Cryptokernel used by this agent
	MainWindow *mainWindow_;	// Main window attached to this agent
	
	
	// Groups maps
	struct GroupInfo {
		group_id_t id;
		GroupItem *item;
		
		QString name;
	};
	typedef std::list<GroupInfo> GroupInfoContainer;
	typedef GroupInfoContainer::iterator groupInfoIterator;
	
	GroupInfoContainer groups_;
	std::unordered_map<group_id_t, groupInfoIterator> groupIdsMap_;
	std::unordered_map<GroupItem *, groupInfoIterator> groupItemsMap_;
	// The root group in mainWindow->leftPanel->groupListWidget availible as
	// groupIdsMap_[kernel_.root_group_id()]
	
	
	// Records maps
	struct RecordInfo {
		record_id_t id;
		RecordItem *item;
		QTreeWidgetItem *recordListItem;
		
		QString name;
	};
	typedef std::list<RecordInfo> RecordInfoContainer;
	typedef RecordInfoContainer::iterator RecordInfoIterator;
	
	RecordInfoContainer records_;
	std::unordered_map<record_id_t, RecordInfoIterator> recordIdsMap_;
	std::unordered_map<RecordItem *, RecordInfoIterator> recordItemsMap_;
	std::unordered_map<QTreeWidgetItem *, RecordInfoIterator> recordListItemsMap_;
	
	// Records are shown in record list in mainWindow->mainWidget->recordListWidget
	// This set is part of set of all "QTreeWidgetItem *"s im recordItemsMap_
	std::unordered_set<QTreeWidgetItem *> shownRecordItems_;
	
	
	// Type maps and root group
	struct TypeInfo {
		type_id_t id;
		TypeItem *item;
		
		QString name;
	};
	typedef std::list<TypeInfo> TypeInfoContainer;
	typedef TypeInfoContainer::iterator TypeInfoIterator;
	
	TypeInfoContainer types_;
	std::unordered_map<type_id_t, TypeInfoIterator> typeIdsMap_;
	std::unordered_map<TypeItem *, TypeInfoIterator> typeItemsMap_;
	GroupItem *rootTypeGroup_;	// Root group of types in mainWindow->leftPanel->groupListWidget
	
	
	// Shown record content
	record_id_t shownRecordId_;
	std::vector<rfield_id_t> shownFieldIds_;
	
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent(const CryptoKernelAgent &other);
	
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent & operator=(const CryptoKernelAgent &other);
};

#endif // CRYPTOKERNELAGENT_H
