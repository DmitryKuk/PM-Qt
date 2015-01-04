// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <iostream>
#include <fstream>
#include <queue>

CryptoKernelAgent::CryptoKernelAgent():
	mainWindow_(nullptr),
	rootTypeGroup_(nullptr)
{
	std::ifstream f("test_input.txt");
	if (!this->kernel_.read(f)) {
		std::cerr << "ERROR: CryptoKernelAgent constructor: Kernel can't read data!" << std::endl;
	} else {
		std::cerr << "DEBUG: OK: CryptoKernelAgent constructor: Kernel read data:" << std::endl;
		if (!this->kernel_.write(std::cerr))
			std::cerr << "DEBUG: ERROR: Kernel can't write data." << std::endl;
	}
}

CryptoKernelAgent::~CryptoKernelAgent()
{
	if (this->mainWindow() != nullptr)
		disconnect(this, this->mainWindow());
}


void CryptoKernelAgent::showData()
{
	if (this->mainWindow_ == nullptr) return;
	
	// Root widget
	QTreeWidget *rootGroupWidget = this->mainWindow_->leftPanelWidget()->groupListWidget();
	
	// Adding groups
	{
		// Root group
		auto rootGroupId = this->kernel_.root_group_id();
		auto rootGroupItem = new GroupItem(QObject::tr("Data"), rootGroupWidget);
		
		this->groupItemsMap_[rootGroupItem] = rootGroupId;
		this->groupIdsMap_[rootGroupId] = rootGroupItem;
		
		
		// Queue for BFS
		std::queue<group_id_t> groupIdQueue;
		groupIdQueue.push(rootGroupId);
		
		while (!groupIdQueue.empty()) {	// Groups
			auto currentGroupId = groupIdQueue.front();
			groupIdQueue.pop();
			
			auto currentGroupItem = this->groupIdsMap_[currentGroupId];
			
			for (auto childGroupId: this->kernel_.groups(currentGroupId)) {
				groupIdQueue.push(childGroupId);
				
				QString childGroupName = QString::fromStdString(this->kernel_.group_name(childGroupId));
				auto childGroupItem = new GroupItem(childGroupName, currentGroupItem);
				
				// Updating agent cached relations
				this->groupItemsMap_[childGroupItem] = childGroupId;
				this->groupIdsMap_[childGroupId] = childGroupItem;
			}
		}
	}
	
	// Adding types
	{
		this->rootTypeGroup_ = new GroupItem(QObject::tr("Types"), rootGroupWidget);
		
		for (auto typeId: this->kernel_.types()) {
			QString typeName = QString::fromStdString(this->kernel_.type_name(typeId));
			TypeItem *typeItem = new TypeItem(typeName, this->rootTypeGroup_);
			
			// Updating agent cached relations
			this->typeItemsMap_[typeItem] = typeId;
			this->typeIdsMap_[typeId] = typeItem;
		}
	}
	
	// Adding records
	{
		auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
		
		for (auto recordId: this->kernel_.records()) {
			auto parentGroupId = this->kernel_.record_parent_group(recordId);
			auto parentGroupItem = this->groupIdsMap_[parentGroupId];
			
			QString recordName = QString::fromStdString(this->kernel_.record_name(recordId));
			RecordItem *recordItem = new RecordItem(recordName, parentGroupItem);
			
			// Adding data into records list
			auto recordListItem = new QTreeWidgetItem(recordListWidget);
			recordListItem->setText(RecordFieldPos::Name, recordName);
			
			auto recordTypeId = this->kernel_.record_type(recordId);
			recordListItem->setText(RecordFieldPos::TypeName,
									this->typeIdsMap_[recordTypeId]->name());
			
			auto recordParentGroupId = this->kernel_.record_parent_group(recordId);
			recordListItem->setText(RecordFieldPos::ParentGroup,
									this->groupIdsMap_[recordParentGroupId]->name());
			
			// Updating agent's cached relations
			this->recordItemsMap_[recordItem] = std::make_pair(recordId, recordListItem);
			this->recordIdsMap_[recordId] = recordItem;
			this->shownRecordItems_.insert(recordItem);
		}
	}
}


void CryptoKernelAgent::updateRecordListItems()
{
	if (this->mainWindow() == nullptr) return;
	std::cerr << "Inside updateRecordListItems: beginning" << std::endl;
	
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	std::cerr << "Inside updateRecordListItems: after groupListWidget" << std::endl;
	auto selectedItemsList = groupListWidget->selectedItems();
	std::cerr << "Inside updateRecordListItems: data here" << std::endl;
	// New set of shown items
	std::unordered_set<QTreeWidgetItem *> oldShownRecordItems;
	oldShownRecordItems.swap(this->shownRecordItems_);
	
	auto showRecordItem = [this, &oldShownRecordItems](RecordItem *recordItem) {
		try {
			auto recordItemInList = this->recordItemsMap_.at(recordItem).second;
			recordItemInList->setHidden(false);					// Showing item
			this->shownRecordItems_.insert(recordItemInList);	// Updating agent's cache
			oldShownRecordItems.erase(recordItemInList);		// DON'T FORGET to update this later
		} catch (...) {}
	};
	std::cerr << "Inside updateRecordListItems: after lambda" << std::endl;
	
	// Queue for searching records in groups (BFS)
	std::queue<group_id_t> groupIdQueue;
	std::cerr << "Inside updateRecordListItems: before cycle" << std::endl;
	for (auto &selectedItem: selectedItemsList) {
		std::cerr << "\tInside cycle" << std::endl;
		auto selectedItemType = dynamic_cast<AbstractItem *>(selectedItem)->type();
		std::cerr << "\tInside cycle, after AbstractItem" << std::endl;
		
		if (selectedItemType == ItemType::Record) {	// Simply showing record item
			showRecordItem(dynamic_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group) {	// Adding group into the queue for processing
			try {
				auto groupItem = dynamic_cast<GroupItem *>(selectedItem);
				auto groupId = this->groupItemsMap_.at(groupItem);
				groupIdQueue.push(groupId);
			} catch (...) {}
		} else if (selectedItemType == ItemType::Type) {	// Showing all records of this type
			try {
				auto typeItem = dynamic_cast<TypeItem *>(selectedItem);
				auto typeId = this->typeItemsMap_.at(typeItem);
				
				auto recordIds = std::move(this->kernel_.records_of_type(typeId));
				for (auto recordId: recordIds) {
					try {
						auto recordItem = this->recordIdsMap_.at(recordId);
						showRecordItem(recordItem);
					} catch (...) {}
				}
			} catch (...) {}
		}	// Else variant is impossible
	}
	
	std::cerr << "Inside updateRecordListItems: after records and types" << std::endl;
	
	// Groups BFS
	while (!groupIdQueue.empty()) {
		auto groupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		try {
			for (auto childGroupId: this->kernel_.groups(groupId))
				groupIdQueue.push(childGroupId);
			for (auto childRecordId: this->kernel_.records(groupId)) {
				auto childRecordItem = this->recordIdsMap_.at(childRecordId);
				showRecordItem(childRecordItem);
			}
		} catch (...) {}
	}
	std::cerr << "Inside updateRecordListItems: after groups" << std::endl;
	
	// Updating agent's cache
	for (auto notSelectedItemInList: oldShownRecordItems)
		notSelectedItemInList->setHidden(true);
	std::cerr << "Inside updateRecordListItems" << std::endl;
}
