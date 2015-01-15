// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"
#include <queue>
#include "algorithm.h"


MainWindow * CryptoKernelAgent::mainWindow()
{ return this->mainWindow_; }


// Update functions
void CryptoKernelAgent::updateRecordListItems()
{
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	auto selectedItemsList = groupListWidget->selectedItems();
	
	// New set of shown items
	std::unordered_set<QTreeWidgetItem *> oldShownRecordListItems;
	oldShownRecordListItems.swap(this->records_.shownRecordListItems);
	
	// Queue for searching records in groups (BFS)
	std::queue<group_id_t> groupIdQueue;
	
	
	// Lambda-helpers
	auto showRecordListItem = [this, &oldShownRecordListItems](RecordItem *recordItem) {
		try {
			auto recordListItem = this->records_.itemsMap.at(recordItem)->recordListItem;
			recordListItem->setHidden(false);					// Showing item
			this->records_.shownRecordListItems.insert(recordListItem);	// Updating agent's cache
			oldShownRecordListItems.erase(recordListItem);		// DON'T FORGET to update this later
		} catch (...) {}
	};
	
	auto showRecordListItemById = [this, &showRecordListItem](record_id_t id) {
		try {
			auto item = this->records_.idsMap.at(id)->item;
			showRecordListItem(item);
		} catch (...) {}
	};
	
	auto showRecordListItemByTypeItem = [this, &showRecordListItemById](TypeItem *typeItem) {
		try {
			auto typeId = this->types_.itemsMap.at(typeItem)->id;
			for_each(this->kernel_.records_of_type(typeId), showRecordListItemById);
		} catch (...) {}
	};
	
	
	auto processGroupId = [this, &groupIdQueue, &showRecordListItemById](group_id_t groupId) {
		for (auto childGroupId: this->kernel_.groups(groupId))
			groupIdQueue.push(childGroupId);
		for_each(this->kernel_.records(groupId), showRecordListItemById);
	};
	
	
	for (auto &selectedItem: selectedItemsList) {
		auto selectedItemType = itemType(selectedItem);
		
		if (selectedItemType == ItemType::Record) {	// Simply showing record item
			showRecordListItem(reinterpret_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group) {	// Adding group into the queue for processing
			auto groupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t groupId;
			try {
				groupId = this->groups_.itemsMap.at(groupItem)->id;
			} catch (...) {}
			processGroupId(groupId);
		} else if (selectedItemType == ItemType::Type) {	// Showing all records of this type
			showRecordListItemByTypeItem(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootGroup) {	// "Data"
			auto rootGroupItem = reinterpret_cast<GroupItem *>(selectedItem);
			group_id_t rootGroupId;
			try {
				rootGroupId = this->groups_.itemsMap.at(rootGroupItem)->id;
			} catch (...) { break; }
			processGroupId(rootGroupId);
		} else if (selectedItemType == ItemType::RootTypeGroup) {	// "Types"
			// Processing all child types (all types)
			for (auto i = selectedItem->childCount() - 1; i >= 0; --i)
				showRecordListItemByTypeItem(reinterpret_cast<TypeItem *>(selectedItem->child(i)));
		}	// Else variant is impossible
	}
	
	// Groups BFS
	while (!groupIdQueue.empty()) {
		auto groupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		processGroupId(groupId);
	}
	
	// Updating agent's cache
	for (auto notSelectedItemInList: oldShownRecordListItems)
		notSelectedItemInList->setHidden(true);
	
	this->updateRecordContent();
}


void CryptoKernelAgent::updateRecordContent()
{
	auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
	auto recordContentWidget = this->mainWindow()->mainWidget()->recordContentWidget();
	recordContentWidget->clear();
	this->recordContent_.clear();
	
	auto selectedRecordItems = recordListWidget->selectedItems();
	if (selectedRecordItems.size() != 1) {	// Selected 0 or >1 items: don't need to show fields
		recordContentWidget->hide();
		return;
	}
	
	try {
		// Record metadata
		auto recordItem = selectedRecordItems[0];
		auto recordId = this->records_.recordListItemsMap.at(recordItem)->id;
		auto recordName = this->records_.idsMap.at(recordId)->name;
		
		auto typeId = this->kernel_.record_type(recordId);
		auto &typeName = this->types_.idsMap.at(typeId)->name;
		
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		auto &parentGroupName = this->groups_.idsMap.at(parentGroupId)->name;
		
		// Filling record metadata
		recordContentWidget->setName(recordName);
		recordContentWidget->setType(typeName);
		recordContentWidget->setGroup(parentGroupName);
		
		// Update agent's cache
		this->recordContent_.shownRecordId = recordId;
		this->recordContent_.shownFieldIds = std::move(this->kernel_.fields(recordId));
		
		// Processing record fields
		QList<QPair<QString, QString>> fieldsToShow;
		for (auto fieldId: this->recordContent_.shownFieldIds) {
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
		auto fieldId = this->recordContent_.shownFieldIds.at(index);
		if (newText.size() == 0) {
			this->kernel_.remove_field(this->recordContent_.shownRecordId, fieldId);
			this->mainWindow()->mainWidget()->recordContentWidget()->removeField(index);
			this->recordContent_.shownFieldIds.erase(this->recordContent_.shownFieldIds.begin() + index);
		} else {
			this->kernel_.set_field_data(this->recordContent_.shownRecordId, fieldId, newText.toStdString());
			this->mainWindow()->mainWidget()->recordContentWidget()->confirmFieldChanges(index);
		}
	} catch (...) {}
}


void CryptoKernelAgent::addGroup()
{
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	auto selectedItems = groupListWidget->selectedItems();
	
	try {
		if (selectedItems.size() != 1) throw (int());
		
		auto selectedItemType = itemType(selectedItems[0]);
		if (selectedItemType != ItemType::Group && selectedItemType != ItemType::RootGroup) throw (int());
		auto selectedItem = reinterpret_cast<GroupItem *>(selectedItems[0]);
		
		auto parentGroupId = this->groups_.itemsMap.at(selectedItem)->id;
		
		// Inserting group like "New group 1"
		QString newGroupName, newGroupNamePrefix = QObject::tr("New group ");
		auto newGroupId = invalid_group_id;
		for (size_t i = 1; newGroupId == invalid_group_id; ++i) {
			newGroupName = newGroupNamePrefix + QString::number(i);
			newGroupId = this->kernel_.add_group(parentGroupId, newGroupName.toStdString());
		}
		
		auto newGroupItem = new GroupItem(newGroupName, selectedItem);
		this->groups_.add({ .id = newGroupId,
							.item = newGroupItem,
							.name = newGroupName });
	} catch (...) {
		this->showWarning(QObject::tr("Error"),
						  QObject::tr("Please, select parent group in list at the left\nand try again."));
	}
}

void CryptoKernelAgent::addRecord()
{
	// auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	// auto selectedItems = groupListWidget->selectedItems();
	
	// try {
	// 	if (selectedItems.size() != 1) throw (int());
		
	// 	auto selectedItemType = itemType(selectedItems[0]);
	// 	if (selectedItemType != ItemType::Group && selectedItemType != ItemType::RootGroup) throw (int());
	// 	auto selectedItem = reinterpret_cast<GroupItem *>(selectedItems[0]);	// Parent group
		
	// 	auto parentGroupId = this->groups_.itemsMap.at(selectedItem)->id;
		
	// 	// Inserting record like "New record 1"
	// 	QString newRecordName, newRecordNamePrefix = QObject::tr("New record ");
	// 	auto newRecordId = invalid_record_id;
	// 	for (size_t i = 1; newRecordId == invalid_record_id; ++i) {
	// 		newRecordName = newRecordNamePrefix + QString::number(i);
	// 		newRecordId = this->kernel_.add_record(parentGroupId, newRecordName.toStdString());
	// 	}
		
	// 	auto newRecordItem = new RecordItem(newRecordName, selectedItem);
	// 	this->records_.add({ .id = newRecordId,
	// 						 .item = newRecordItem,
	// 						 .name = newRecordName });
	// } catch (...) {
	// 	this->showWarning(QObject::tr("Error"),
	// 					  QObject::tr("Please, select parent group in list at the left\nand try again."));
	// }
}

void CryptoKernelAgent::addType()
{
	// Inserting type like "New type 1"
	QString newTypeName, newTypeNamePrefix = QObject::tr("New type ");
	auto newTypeId = invalid_type_id;
	for (size_t i = 1; newTypeId == invalid_type_id; ++i) {
		newTypeName = newTypeNamePrefix + QString::number(i);
		newTypeId = this->kernel_.add_type(newTypeName.toStdString());
	}
	
	auto newTypeItem = new TypeItem(newTypeName, this->types_.rootGroup);
	this->types_.add({ .id = newTypeId,
					   .item = newTypeItem,
					   .name = newTypeName });
}


void CryptoKernelAgent::removeSelectedItems()
{
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	auto selectedItemsList = groupListWidget->selectedItems();
	
	for (auto &selectedItem: selectedItemsList) {
		auto selectedItemType = itemType(selectedItem);
		
		std::cerr << "HERE: type = " << selectedItemType << std::endl;
		
		if (selectedItemType == ItemType::Record) {
			this->removeRecord(reinterpret_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group || selectedItemType == ItemType::RootGroup) {
			std::cerr << "Deleting group" << std::endl;
			this->removeGroup(reinterpret_cast<GroupItem *>(selectedItem));
			std::cerr << "Group deleted" << std::endl;
		} else if (selectedItemType == ItemType::Type) {
			this->removeType(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootTypeGroup) {	// "Types"
			// Processing all child types (all types)
			for (auto i = selectedItem->childCount() - 1; i >= 0; --i)
				this->removeType(reinterpret_cast<TypeItem *>(selectedItem->child(i)));
		}	// Else variant is impossible
	}
}


void CryptoKernelAgent::mainWindowClosed()
{ this->closeWarning(); }


void CryptoKernelAgent::showWarning(const QString &title, const QString &text)
{
	this->warningWindow_->setTitle(title);
	this->warningWindow_->setText(text);
	this->warningWindow_->show();
}

void CryptoKernelAgent::closeWarning()
{ this->warningWindow_->close(); }