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
	delete this->mainWindow_;
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
			auto recordListItem = this->recordItemsMap_.at(recordItem).second;
			recordListItem->setHidden(false);				// Showing item
			this->shownRecordItems_.insert(recordListItem);	// Updating agent's cache
			oldShownRecordItems.erase(recordListItem);		// DON'T FORGET to update this later
		} catch (...) {}
	};
	
	auto showRecordsByTypeItem = [this, &showRecordItem](TypeItem *typeItem) {
		try {
			auto typeId = this->typeItemsMap_.at(typeItem);
			
			auto recordIds = std::move(this->kernel_.records_of_type(typeId));
			for (auto recordId: recordIds) {
				try {
					auto recordItem = this->recordIdsMap_.at(recordId);
					showRecordItem(recordItem);
				} catch (...) {}
			}
		} catch (...) {}
	};
	
	
	auto processGroupId = [this, &groupIdQueue, &showRecordItem](group_id_t groupId) {
		for (auto childGroupId: this->kernel_.groups(groupId))
			groupIdQueue.push(childGroupId);
		for (auto childRecordId: this->kernel_.records(groupId)) {
			try {
				auto childRecordItem = this->recordIdsMap_.at(childRecordId);
				showRecordItem(childRecordItem);
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
				groupId = this->groupItemsMap_.at(groupItem);
			} catch (...) {}
			processGroupId(groupId);
		} else if (selectedItemType == ItemType::Type) {	// Showing all records of this type
			showRecordsByTypeItem(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootGroup) {	// "Data"
			auto rootGroupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t rootGroupId;
			try {
				rootGroupId = this->groupItemsMap_.at(rootGroupItem);
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
}


void CryptoKernelAgent::updateRecordContent()
{
	if (this->mainWindow() == nullptr) return;
	
	auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
	auto recordContentWidget = this->mainWindow()->mainWidget()->recordContentWidget();
	recordContentWidget->clear();
	
	auto selectedRecordItems = recordListWidget->selectedItems();
	if (selectedRecordItems.size() != 1) return;	// Selected 0 or >1 items: don't need to show fields
	
	try {
		// Record metadata
		auto recordItem = selectedRecordItems[0];
		auto recordId = this->recordListItemsMap_.at(recordItem);
		auto recordName = this->recordIdsMap_.at(recordId)->name();
		auto typeId = this->kernel_.record_type(recordId);
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		
		const QString &typeName = this->typeIdsMap_.at(typeId)->name();
		const QString &parentGroupName = this->groupIdsMap_.at(parentGroupId)->name();
		
		// Filling record metadata
		recordContentWidget->setName(recordName);
		recordContentWidget->setType(typeName);
		recordContentWidget->setGroup(parentGroupName);
		
		// Update agent's cache
		this->shownRecordId_ = recordId;
		this->shownFieldIds_ = this->kernel_.fields(recordId);
		
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
				childGroupItem->setData(0, Qt::UserRole, ItemType::Group);
				
				// Updating agent cached relations
				this->groupItemsMap_[childGroupItem] = childGroupId;
				this->groupIdsMap_[childGroupId] = childGroupItem;
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
			TypeItem *typeItem = new TypeItem(typeName, this->rootTypeGroup_);
			typeItem->setData(0, Qt::UserRole, ItemType::Type);
			
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
			
			
			// Adding data into groupsListWidget
			QString recordName = QString::fromStdString(this->kernel_.record_name(recordId));
			RecordItem *recordItem = new RecordItem(recordName, parentGroupItem);
			recordItem->setData(0, Qt::UserRole, ItemType::Record);
			
			
			// Adding data into recordListWidget
			auto recordListItem = new QTreeWidgetItem(recordListWidget);
			recordListItem->setText(RecordFieldPos::Name,
									  recordName);
			
			auto recordTypeId = this->kernel_.record_type(recordId);
			recordListItem->setText(RecordFieldPos::TypeName,
									  this->typeIdsMap_[recordTypeId]->name());
			
			auto recordParentGroupId = this->kernel_.record_parent_group(recordId);
			recordListItem->setText(RecordFieldPos::ParentGroup,
									  this->groupIdsMap_[recordParentGroupId]->name());
			
			
			// Updating agent's cached relations
			this->recordItemsMap_[recordItem] = std::make_pair(recordId, recordListItem);
			this->recordIdsMap_[recordId] = recordItem;
			this->recordListItemsMap_[recordListItem] = recordId;
			this->shownRecordItems_.insert(recordListItem);
		}
	}
}


QString CryptoKernelAgent::typeName(RecordItem *recordItem) const
{
	try {
		auto recordId = this->recordItemsMap_.at(recordItem).first;
		auto typeId = this->kernel_.record_type(recordId);
		auto typeItem = this->typeIdsMap_.at(typeId);
		return typeItem->name();
	} catch (...) {}
	return QString();
}

QString CryptoKernelAgent::parentGroupName(RecordItem *recordItem) const
{
	try {
		auto recordId = this->recordItemsMap_.at(recordItem).first;
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		auto parentGroupItem = this->groupIdsMap_.at(parentGroupId);
		return parentGroupItem->name();
	} catch (...) {}
	return QString();
}


void CryptoKernelAgent::showRecordInList(RecordItem *recordItem)
{
	try {
		this->recordItemsMap_.at(recordItem).second->setHidden(false);
	} catch (...) {}
}

void CryptoKernelAgent::hideRecordInList(RecordItem *recordItem)
{
	try {
		this->recordItemsMap_.at(recordItem).second->setHidden(true);
	} catch (...) {}
}
