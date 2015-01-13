// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <iostream>
#include <fstream>
#include <queue>

// Constants for identification of selected elements in groupListWidget
// Set one of these with setData(0, Qt::UserRole, oneOfThese)
namespace ItemType {
static const int
	Record			= 10,
	Group			= 20,
	Type			= 30,
	RootGroup		= 40,
	RootTypeGroup	= 50;
};


CryptoKernelAgent::CryptoKernelAgent():
	mainWindow_(new MainWindow(this)),
	rootTypeGroup_(nullptr)
{
	std::ifstream f("test_input.txt");
	if (!this->kernel_.read(f))
		std::cerr << "ERROR: CryptoKernelAgent constructor: Kernel can't read data!" << std::endl;
}

CryptoKernelAgent::~CryptoKernelAgent()
{
	delete this->mainWindow_;
	
	std::ofstream f("test_input.txt");
	if (!this->kernel_.write(f))
		std::cerr << "ERROR: CryptoKernelAgent destructor: Kernel can't write data!" << std::endl;
}


MainWindow * CryptoKernelAgent::mainWindow() const
{ return this->mainWindow_; }


void CryptoKernelAgent::run()
{
	this->showData();
	this->mainWindow()->show();
	this->mainWindow()->mainWidget()->recordContentWidget()->hide();
}


// Slots
// Update functions
void CryptoKernelAgent::updateRecordListItems()
{
	if (this->mainWindow() == nullptr) return;
	
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	auto selectedItemsList = groupListWidget->selectedItems();
	
	// New set of shown items
	std::unordered_set<QTreeWidgetItem *> oldShownRecordItems;
	oldShownRecordItems.swap(this->shownRecordItems_);
	
	// Queue for searching records in groups (BFS)
	std::queue<group_id_t> groupIdQueue;
	
	
	// Lambda-helpers
	auto showRecordItem = [this, &oldShownRecordItems](RecordItem *recordItem) {
		try {
			auto recordListItem = this->recordItemsMap_.at(recordItem)->recordListItem;
			recordListItem->setHidden(false);				// Showing item
			this->shownRecordItems_.insert(recordListItem);	// Updating agent's cache
			oldShownRecordItems.erase(recordListItem);		// DON'T FORGET to update this later
		} catch (...) {}
	};
	
	auto showRecordsByTypeItem = [this, &showRecordItem](TypeItem *typeItem) {
		try {
			auto typeId = this->typeItemsMap_.at(typeItem)->id;
			
			auto recordIds = std::move(this->kernel_.records_of_type(typeId));
			for (auto recordId: recordIds) {
				try {
					auto childRecordListItem = this->recordIdsMap_.at(recordId)->recordListItem;
					childRecordListItem->setHidden(false);
					this->shownRecordItems_.insert(childRecordListItem);
				} catch (...) {}
			}
		} catch (...) {}
	};
	
	
	auto processGroupId = [this, &groupIdQueue, &showRecordItem](group_id_t groupId) {
		for (auto childGroupId: this->kernel_.groups(groupId))
			groupIdQueue.push(childGroupId);
		for (auto childRecordId: this->kernel_.records(groupId)) {
			try {
				auto childRecordListItem = this->recordIdsMap_.at(childRecordId)->recordListItem;
				childRecordListItem->setHidden(false);
				this->shownRecordItems_.insert(childRecordListItem);
			} catch (...) {}
		}
	};
	
	
	for (auto &selectedItem: selectedItemsList) {
		int selectedItemType = selectedItem->data(0, Qt::UserRole).toInt();
		
		if (selectedItemType == ItemType::Record) {	// Simply showing record item
			showRecordItem(reinterpret_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group) {	// Adding group into the queue for processing
			auto groupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t groupId;
			try {
				groupId = this->groupItemsMap_.at(groupItem)->id;
			} catch (...) {}
			processGroupId(groupId);
		} else if (selectedItemType == ItemType::Type) {	// Showing all records of this type
			showRecordsByTypeItem(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootGroup) {	// "Data"
			auto rootGroupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t rootGroupId;
			try {
				rootGroupId = this->groupItemsMap_.at(rootGroupItem)->id;
			} catch (...) { break; }
			processGroupId(rootGroupId);
		} else if (selectedItemType == ItemType::RootTypeGroup) {	// "Types"
			// Processing all child types (all types)
			for (auto i = selectedItem->childCount() - 1; i >= 0; --i)
				showRecordsByTypeItem(reinterpret_cast<TypeItem *>(selectedItem->child(i)));
		}	// Else variant is impossible
	}
	
	// Groups BFS
	while (!groupIdQueue.empty()) {
		auto groupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		processGroupId(groupId);
	}
	
	// Updating agent's cache
	for (auto notSelectedItemInList: oldShownRecordItems)
		notSelectedItemInList->setHidden(true);
	
	this->updateRecordContent();
}


void CryptoKernelAgent::updateRecordContent()
{
	if (this->mainWindow() == nullptr) return;
	
	auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
	auto recordContentWidget = this->mainWindow()->mainWidget()->recordContentWidget();
	recordContentWidget->clear();
	
	auto selectedRecordItems = recordListWidget->selectedItems();
	if (selectedRecordItems.size() != 1) {	// Selected 0 or >1 items: don't need to show fields
		recordContentWidget->hide();
		return;
	}
	
	try {
		// Record metadata
		auto recordItem = selectedRecordItems[0];
		auto recordId = this->recordListItemsMap_.at(recordItem)->id;
		auto recordName = this->recordIdsMap_.at(recordId)->name;
		
		auto typeId = this->kernel_.record_type(recordId);
		auto &typeName = this->typeIdsMap_.at(typeId)->name;
		
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		auto &parentGroupName = this->groupIdsMap_.at(parentGroupId)->name;
		
		// Filling record metadata
		recordContentWidget->setName(recordName);
		recordContentWidget->setType(typeName);
		recordContentWidget->setGroup(parentGroupName);
		
		// Update agent's cache
		this->shownRecordId_ = recordId;
		this->shownFieldIds_ = std::move(this->kernel_.fields(recordId));
		
		// Processing record fields
		QList<QPair<QString, QString>> fieldsToShow;
		for (auto fieldId: this->shownFieldIds_) {
			auto fieldTypeId = this->kernel_.field_type(recordId, fieldId);
			auto fieldTypeName = QString::fromStdString(this->kernel_.type_field_name(typeId, fieldTypeId));
			auto fieldData = QString::fromStdString(this->kernel_.field_data(recordId, fieldId));
			
			fieldsToShow.append(qMakePair(fieldTypeName, fieldData));
		}
		recordContentWidget->setFields(fieldsToShow);
		
		recordContentWidget->show();
	} catch (...) {
		recordContentWidget->clear();
		recordContentWidget->hide();
	}
}


// Slots
void CryptoKernelAgent::onNameClicked()
{
	auto name = this->mainWindow()->mainWidget()->recordContentWidget()->name();
	std::cerr << "Copy name \"" << name.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::onNameChanged(QString newName)
{
	std::cerr << "Inside CryptoKernelAgent::onNameChanged(\"" << newName.toStdString() << "\"): NOT IMPLEMENTED" << std::endl;
}

void CryptoKernelAgent::onTypeClicked()
{
	auto typeName = this->mainWindow()->mainWidget()->recordContentWidget()->type();
	std::cerr << "Copy type \"" << typeName.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::onTypeChanged(QString newTypeName)
{
	std::cerr << "Inside CryptoKernelAgent::onTypeChanged(\"" << newTypeName.toStdString() << "\"): NOT IMPLEMENTED" << std::endl;
}

void CryptoKernelAgent::onGroupClicked()
{
	auto groupName = this->mainWindow()->mainWidget()->recordContentWidget()->group();
	std::cerr << "Copy group \"" << groupName.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::onGroupChanged(QString newGroupName)
{
	std::cerr << "Inside CryptoKernelAgent::onGroupChanged(\"" << newGroupName.toStdString() << "\"): NOT IMPLEMENTED" << std::endl;
}

void CryptoKernelAgent::onFieldClicked(int index)
{
	auto fieldData = this->mainWindow()->mainWidget()->recordContentWidget()->originalField(index);
	std::cerr << "Copy field \"" << fieldData.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::onFieldChanged(int index, QString newText)
{
	try {
		auto fieldId = this->shownFieldIds_.at(index);
		if (newText.size() == 0) {
			this->kernel_.remove_field(this->shownRecordId_, fieldId);
			this->mainWindow()->mainWidget()->recordContentWidget()->removeField(index);
			
			// Updating agent's cache
			this->shownFieldIds_.erase(this->shownFieldIds_.begin() + index);
		} else {
			this->kernel_.set_field_data(this->shownRecordId_, fieldId, newText.toStdString());
			this->mainWindow()->mainWidget()->recordContentWidget()->confirmFieldChanges(index);
		}
	} catch (...) {}
}


void CryptoKernelAgent::addGroup()
{
	if (this->mainWindow_ == nullptr) return;
}

void CryptoKernelAgent::addRecord()
{
	if (this->mainWindow_ == nullptr) return;
	std::cerr << "HERE!" << std::endl;
}

void CryptoKernelAgent::addType()
{
	if (this->mainWindow_ == nullptr) return;
	
}

void CryptoKernelAgent::remove()
{
	if (this->mainWindow() == nullptr) return;
	
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	auto selectedItemsList = groupListWidget->selectedItems();
	
	// Queue for searching records in groups (BFS)
	std::queue<group_id_t> groupIdQueue;
	
	
	// Lambda-helpers
	auto removeRecordItem = [this](RecordItem *recordItem) {
		try {
			auto iterator = this->recordItemsMap_.at(recordItem);
			this->kernel_.remove_record(iterator->id);
			
			this->shownRecordItems_.erase(iterator->recordListItem);
			this->recordIdsMap_.erase(iterator->id);
			this->recordItemsMap_.erase(iterator->item);
			this->recordListItemsMap_.erase(iterator->recordListItem);
			
			delete iterator->recordListItem;
			delete iterator->item;
			
			this->records_.erase(iterator);
		} catch (...) {}
	};
	
	auto removeRecordsByTypeItem = [this, &removeRecordItem](TypeItem *typeItem) {
		try {
			auto typeId = this->typeItemsMap_.at(typeItem)->id;
			
			auto recordIds = std::move(this->kernel_.records_of_type(typeId));
			for (auto recordId: recordIds) {
				try {
					removeRecordItem(this->recordIdsMap_.at(recordId)->item);
				} catch (...) {}
			}
			
			auto iterator = this->typeItemsMap_.at(typeItem);
			this->kernel_.remove_type(iterator->id);
			delete iterator->item;
			this->typeIdsMap_.erase(iterator->id);
			this->typeItemsMap_.erase(iterator->item);
			this->types_.erase(iterator);
		} catch (...) {}
	};
	
	
	auto processGroupId = [this, &groupIdQueue, &removeRecordItem](group_id_t groupId) {
		for (auto childGroupId: this->kernel_.groups(groupId))
			groupIdQueue.push(childGroupId);
		for (auto childRecordId: this->kernel_.records(groupId)) {
			try {
				removeRecordItem(this->recordIdsMap_.at(childRecordId)->item);
			} catch (...) {}
		}
		
		try {
			auto iterator = this->groupIdsMap_.at(groupId);
			this->kernel_.remove_group(iterator->id);
			delete iterator->item;
			this->groupIdsMap_.erase(iterator->id);
			this->groupItemsMap_.erase(iterator->item);
			this->groups_.erase(iterator);
		} catch (...) {}
	};
	
	
	for (auto &selectedItem: selectedItemsList) {
		int selectedItemType = selectedItem->data(0, Qt::UserRole).toInt();
		
		if (selectedItemType == ItemType::Record) {	// Simply showing record item
			removeRecordItem(reinterpret_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group) {	// Adding group into the queue for processing
			auto groupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t groupId;
			try {
				groupId = this->groupItemsMap_.at(groupItem)->id;
			} catch (...) {}
			processGroupId(groupId);
		} else if (selectedItemType == ItemType::Type) {	// Showing all records of this type
			removeRecordsByTypeItem(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootGroup) {	// "Data"
			auto rootGroupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t rootGroupId;
			try {
				rootGroupId = this->groupItemsMap_.at(rootGroupItem)->id;
			} catch (...) { break; }
			processGroupId(rootGroupId);
		} else if (selectedItemType == ItemType::RootTypeGroup) {	// "Types"
			// Processing all child types (all types)
			for (auto i = selectedItem->childCount() - 1; i >= 0; --i)
				removeRecordsByTypeItem(reinterpret_cast<TypeItem *>(selectedItem->child(i)));
		}	// Else variant is impossible
	}
	
	// Groups BFS
	while (!groupIdQueue.empty()) {
		auto groupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		processGroupId(groupId);
	}
	
	this->updateRecordContent();
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
		rootGroupItem->setData(0, Qt::UserRole, ItemType::RootGroup);
		rootGroupItem->setExpanded(true);
		
		{
			auto iterator = this->groups_.insert(this->groups_.end(), { .id = rootGroupId,
																		.item = rootGroupItem,
																		.name = QObject::tr("Data") });
			
			this->groupItemsMap_[rootGroupItem] = iterator;
			this->groupIdsMap_[rootGroupId] = iterator;
		}
		
		
		// Queue for BFS
		std::queue<group_id_t> groupIdQueue;
		groupIdQueue.push(rootGroupId);
		
		while (!groupIdQueue.empty()) {	// Groups
			auto currentGroupId = groupIdQueue.front();
			groupIdQueue.pop();
			
			auto currentGroupItem = this->groupIdsMap_[currentGroupId]->item;
			
			for (auto childGroupId: this->kernel_.groups(currentGroupId)) {
				groupIdQueue.push(childGroupId);
				
				QString childGroupName = QString::fromStdString(this->kernel_.group_name(childGroupId));
				auto childGroupItem = new GroupItem(childGroupName, currentGroupItem);
				childGroupItem->setData(0, Qt::UserRole, ItemType::Group);
				
				// Updating agent's cached relations
				auto iterator = this->groups_.insert(this->groups_.end(), { .id = childGroupId,
																			.item = childGroupItem,
																			.name = childGroupName });
				this->groupIdsMap_[childGroupId] = iterator;
				this->groupItemsMap_[childGroupItem] = iterator;
			}
		}
	}
	
	// Adding types
	{
		this->rootTypeGroup_ = new GroupItem(QObject::tr("Types"), rootGroupWidget);
		this->rootTypeGroup_->setData(0, Qt::UserRole, ItemType::RootTypeGroup);
		this->rootTypeGroup_->setExpanded(true);
		
		for (auto typeId: this->kernel_.types()) {
			QString typeName = QString::fromStdString(this->kernel_.type_name(typeId));
			auto typeItem = new TypeItem(typeName, this->rootTypeGroup_);
			typeItem->setData(0, Qt::UserRole, ItemType::Type);
			
			// Updating agent's cached relations
			auto iterator = this->types_.insert(this->types_.end(), { .id = typeId,
																	  .item = typeItem,
																	  .name = typeName });
			this->typeIdsMap_[typeId] = iterator;
			this->typeItemsMap_[typeItem] = iterator;
		}
	}
	
	// Adding records
	{
		auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
		
		for (auto recordId: this->kernel_.records()) {
			auto parentGroupId = this->kernel_.record_parent_group(recordId);
			auto parentGroupItem = this->groupIdsMap_[parentGroupId]->item;
			
			
			// Adding data into groupsListWidget
			QString recordName = QString::fromStdString(this->kernel_.record_name(recordId));
			auto recordItem = new RecordItem(recordName, parentGroupItem);
			recordItem->setData(0, Qt::UserRole, ItemType::Record);
			
			
			// Adding data into recordListWidget
			auto recordListItem = new QTreeWidgetItem(recordListWidget);
			recordListItem->setText(RecordFieldPos::Name,
									  recordName);
			
			auto recordTypeId = this->kernel_.record_type(recordId);
			recordListItem->setText(RecordFieldPos::TypeName,
									  this->typeIdsMap_[recordTypeId]->name);
			
			auto recordParentGroupId = this->kernel_.record_parent_group(recordId);
			recordListItem->setText(RecordFieldPos::ParentGroup,
									  this->groupIdsMap_[recordParentGroupId]->name);
			
			
			// Updating agent's cached relations
			auto iterator = this->records_.insert(this->records_.end(), { .id = recordId,
																		  .item = recordItem,
																		  .recordListItem = recordListItem,
																		  .name = recordName });
			this->recordIdsMap_[recordId] = iterator;
			this->recordItemsMap_[recordItem] = iterator;
			this->recordListItemsMap_[recordListItem] = iterator;
			
			this->shownRecordItems_.insert(recordListItem);
		}
	}
}


QString CryptoKernelAgent::typeName(RecordItem *recordItem) const
{
	try {
		return this->recordItemsMap_.at(recordItem)->name;
	} catch (...) {}
	return QString();
}

QString CryptoKernelAgent::parentGroupName(RecordItem *recordItem) const
{
	try {
		auto parentGroupId = this->kernel_.record_parent_group(this->recordItemsMap_.at(recordItem)->id);
		return this->groupIdsMap_.at(parentGroupId)->name;
	} catch (...) {}
	return QString();
}


void CryptoKernelAgent::showRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->recordItemsMap_.at(recordItem)->recordListItem;
		recordListItem->setHidden(false);
		this->shownRecordItems_.insert(recordListItem);
	} catch (...) {}
}

void CryptoKernelAgent::hideRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->recordItemsMap_.at(recordItem)->recordListItem;
		recordListItem->setHidden(true);
		this->shownRecordItems_.erase(recordListItem);
	} catch (...) {}
}
