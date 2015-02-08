// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains cryptokernel agent. Agent must use Kernel for all kinds
// of cryptography tasks: generate new ids, encode or decode data...
// Agent is interface for GUI to do all cryptography actions.

// Example: [     GUI      ] <-> [          Agent          ] <-> [    Kernel    ]
//          (Qt/ncurses/...)     (depends on GUI and Kernel)     (lib/daemon/...)
// This version of program uses Qt GUI, so Agent has Qt-based API too. Agent may has
// own cache for some data.

#ifndef CRYPTOKERNELAGENT_H
#define CRYPTOKERNELAGENT_H

#include <QObject>
#include <QString>

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
	
  // Other functions
	void run();
  // End of other functions
	
  // GUI management
	inline MainWindow * GUI_mainWindow() const;
	
	// Update functions
	void GUI_updateRecordListItems();
	void GUI_updateRecordContent();
	
	// Record content
	void GUI_onRecordNameChanged(QString newName);
	void GUI_onRecordTypeNameClicked();
	void GUI_onRecordGroupNameClicked();
	
	void GUI_onRecordFieldClicked(types::rfield_id fieldId);
	void GUI_onRecordFieldChanged(types::rfield_id fieldId, QString newText);
	
	void GUI_addRecordField();
	void GUI_setRecordFieldType(types::rfield_id fieldId, types::tfield_id typeFieldId);
	
	// Group list
	void GUI_onItemDataChanged(QTreeWidgetItem *item, int index);	// Group, record or type name changed
	
	// Add group/record/type, remove selected
	void GUI_addGroup();
	void GUI_addRecord();
	void GUI_addType();
	void GUI_removeSelectedItems();
	
	// Show warning with text
	void GUI_showWarning(const QString &title, const QString &text);
	
	void GUI_showTypeEditDialog();
  // End of GUI management
private:
  // GUI management (see cryptokernelagent_gui.cpp)
	MainWindow *mainWindow_;		// Main window attached to this agent
  // End of GUI management
	
	
  // Data management (see cryptokernelagent_data.cpp)
	cryptokernel *kernel_;			// Cryptokernel used by this agent
	
	
	QString DATA_typeName(RecordItem *recordItem) const;
	QString DATA_parentGroupName(RecordItem *recordItem) const;
	
	void DATA_showRecordInList(RecordItem *recordItem);
	void DATA_hideRecordInList(RecordItem *recordItem);
	
	
	struct Groups {	// Groups maps
		struct GroupInfo {
			types::group_id id;
			GroupItem *item;
			
			QString name;
		};
		typedef std::list<GroupInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<types::group_id, iterator> idsMap;
		std::unordered_map<GroupItem *, iterator> itemsMap;
		// The root group in mainWindow->leftPanel->groupListWidget availible as
		// groupIdsMap[kernel_.root_group_id()]
		
		
		bool add(const GroupInfo &info);
		void erase(types::group_id id);
		void erase(GroupItem *item);
		
		inline bool cached(types::group_id id);
		inline bool cached(GroupItem *item);
		
		void clear();
	};	// struct Groups
	Groups groups_;
	
	bool DATA_loadGroup(types::group_id id);
	void DATA_loadGroups();	// Unsafe! Use loadData()!
	
	void DATA_addRootGroup();	// Unsafe! Use loadData()!
	void DATA_removeRootGroup();	// Unsafe! Use removeSelectedItems() or removeGroup()!
	void DATA_removeGroup(GroupItem *item);
	
	bool DATA_groupItemNameChanged(GroupItem *item);
	
	
	struct Records {	// Records maps and set of shown items
		struct RecordInfo {
			types::record_id id;
			RecordItem *item;
			QTreeWidgetItem *recordListItem;
			
			QString name;
		};
		typedef std::list<RecordInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<types::record_id, iterator> idsMap;
		std::unordered_map<RecordItem *, iterator> itemsMap;
		std::unordered_map<QTreeWidgetItem *, iterator> recordListItemsMap;
		
		
		// Records are shown in record list in mainWindow->mainWidget->recordListWidget
		// This set is part of set of all "QTreeWidgetItem *"s im recordItemsMap
		std::unordered_set<QTreeWidgetItem *> shownRecordListItems;
		
		
		bool add(const RecordInfo &info);
		void erase(types::record_id id);
		void erase(RecordItem *item);
		void erase(QTreeWidgetItem *recordListItem);
		
		inline bool cached(types::record_id id);
		inline bool cached(RecordItem *item);
		
		void clear();
	};	// struct Records
	Records records_;
	
	bool DATA_loadRecord(types::record_id id);
	void DATA_loadRecords();	// Unsafe! Use loadData()!
	
	void DATA_addRecordField();
	void DATA_setRecordFieldType(types::rfield_id fieldId, types::tfield_id typeFieldId);
	
	void DATA_removeRecord(RecordItem *item);
	
	bool DATA_recordItemNameChanged(RecordItem *item);
	
	
	struct Types {	// Type maps and root group
		struct TypeInfo {
			types::type_id id;
			TypeItem *item;
			
			QString name;
		};
		typedef std::list<TypeInfo> Container;
		typedef Container::iterator iterator;
		
		
		Container list;
		std::unordered_map<types::type_id, iterator> idsMap;
		std::unordered_map<TypeItem *, iterator> itemsMap;
		GroupItem *rootGroup = nullptr;	// Root group of types in mainWindow->leftPanel->groupListWidget
		
		
		bool add(const TypeInfo &info);
		void erase(types::type_id id);
		void erase(TypeItem *item);
		
		inline bool cached(types::type_id id);
		inline bool cached(TypeItem *item);
		
		void clear();
	};	// struct Types
	Types types_;
	
	bool DATA_loadType(types::type_id id);
	void DATA_loadTypes();	// Unsafe! Use loadData()!
	
	void DATA_removeType(TypeItem *item);
	void DATA_removeTypeField(TypeItem *item, types::tfield_id fieldId);
	
	bool DATA_typeItemNameChanged(TypeItem *item);
	
	
	void DATA_loadData();	// Safe
  // End of data management
	
	
	// Deprecated constructor and operator=()
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent(const CryptoKernelAgent &other);
	
	[[deprecated("Don't copy the agent!")]]
	CryptoKernelAgent & operator=(const CryptoKernelAgent &other);
};


#include "cryptokernelagent_data.hpp"
#include "cryptokernelagent_gui.hpp"
#endif // CRYPTOKERNELAGENT_H
