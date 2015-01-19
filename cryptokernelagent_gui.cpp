// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <QMessageBox>

#include <queue>

#include "algorithm.h"
#include "typeeditdialog.h"


// Update functions
void CryptoKernelAgent::GUI_updateRecordListItems()
{
	auto groupListWidget = this->GUI_mainWindow()->groupListWidget();
	auto selectedItems = groupListWidget->selectedItems();
	
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
			for_each(this->kernel_->records_of_type(typeId), showRecordListItemById);
		} catch (...) {}
	};
	
	
	auto processGroupId = [this, &groupIdQueue, &showRecordListItemById](group_id_t groupId) {
		for (auto childGroupId: this->kernel_->groups(groupId))
			groupIdQueue.push(childGroupId);
		for_each(this->kernel_->records(groupId), showRecordListItemById);
	};
	
	
	for (auto &selectedItem: selectedItems) {
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
	
	this->GUI_updateRecordContent();
}


void CryptoKernelAgent::GUI_updateRecordContent()
{
	auto recordListWidget = this->GUI_mainWindow()->recordListWidget();
	auto recordContentWidget = this->GUI_mainWindow()->recordContentWidget();
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
		
		auto typeId = this->kernel_->record_type(recordId);
		auto &typeName = this->types_.idsMap.at(typeId)->name;
		
		auto parentGroupId = this->kernel_->record_parent_group(recordId);
		auto &parentGroupName = this->groups_.idsMap.at(parentGroupId)->name;
		
		// Filling record metadata
		recordContentWidget->setName(recordName);
		recordContentWidget->setTypeName(typeName);
		recordContentWidget->setGroupName(parentGroupName);
		
		// Update agent's cache
		this->recordContent_.shownRecordId = recordId;
		this->recordContent_.shownFieldIds = std::move(this->kernel_->fields(recordId));
		
		// Processing record fields
		QList<QPair<QString, QString>> fieldsToShow;
		for (auto fieldId: this->recordContent_.shownFieldIds) {
			auto fieldTypeId = this->kernel_->field_type(recordId, fieldId);
			auto fieldTypeName = QString::fromStdString(this->kernel_->type_field_name(typeId, fieldTypeId));
			auto fieldData = QString::fromStdString(this->kernel_->field_data(recordId, fieldId));
			
			fieldsToShow.append(qMakePair(fieldTypeName, fieldData));
		}
		recordContentWidget->setFields(fieldsToShow);
		
		recordContentWidget->show();
	} catch (...) {
		recordContentWidget->clear();
		recordContentWidget->hide();
	}
}


// Record content
void CryptoKernelAgent::GUI_onRecordNameChanged(QString newName)
{
	try {
		auto shownRecordItem = this->records_.idsMap.at(this->recordContent_.shownRecordId)->item;
		shownRecordItem->setName(newName);
		if (this->DATA_recordItemNameChanged(shownRecordItem))
			this->GUI_mainWindow()->recordContentWidget()->confirmNameChanges();
	} catch (...) {}
}

void CryptoKernelAgent::GUI_onRecordTypeNameClicked()
{
	auto typeName = this->GUI_mainWindow()->recordContentWidget()->typeName();
	std::cerr << "Copy type name \"" << typeName.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::GUI_onRecordGroupNameClicked()
{
	auto groupName = this->GUI_mainWindow()->recordContentWidget()->groupName();
	std::cerr << "Copy group name \"" << groupName.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::GUI_onRecordFieldClicked(int index)
{
	auto fieldData = this->GUI_mainWindow()->recordContentWidget()->originalField(index);
	std::cerr << "Copy field data \"" << fieldData.toStdString() << "\"." << std::endl;
}

void CryptoKernelAgent::GUI_onRecordFieldChanged(int index, QString newText)
{
	try {
		auto fieldId = this->recordContent_.shownFieldIds.at(index);
		if (newText.size() == 0) {
			this->kernel_->remove_field(this->recordContent_.shownRecordId, fieldId);
			this->GUI_mainWindow()->recordContentWidget()->removeField(index);
			this->recordContent_.shownFieldIds.erase(this->recordContent_.shownFieldIds.begin() + index);
		} else {
			this->kernel_->set_field_data(this->recordContent_.shownRecordId, fieldId, newText.toStdString());
			this->GUI_mainWindow()->recordContentWidget()->confirmFieldChanges(index);
		}
	} catch (...) {}
}


void CryptoKernelAgent::GUI_addRecordField()
{ this->DATA_addRecordField(); }


// Group list
void CryptoKernelAgent::GUI_onItemDataChanged(QTreeWidgetItem *item, int index)
{
	if (index != 0) return;
	
	auto type = itemType(item);
	if (type == ItemType::Group)
		this->DATA_groupItemNameChanged(reinterpret_cast<GroupItem *>(item));
	else if (type == ItemType::Record)
		this->DATA_recordItemNameChanged(reinterpret_cast<RecordItem *>(item));
	else if (type == ItemType::Type)
		this->DATA_typeItemNameChanged(reinterpret_cast<TypeItem *>(item));
}


void CryptoKernelAgent::GUI_addGroup()
{
	auto groupListWidget = this->GUI_mainWindow()->groupListWidget();
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
			newGroupId = this->kernel_->add_group(parentGroupId, newGroupName.toStdString());
		}
		
		this->DATA_loadGroup(newGroupId);
	} catch (...) {
		this->GUI_showWarning(QObject::tr("Error"),
						  QObject::tr("Please, select parent group in list at the left "
						  			  "and try again."));
	}
}

void CryptoKernelAgent::GUI_addRecord()
{
	auto groupListWidget = this->GUI_mainWindow()->groupListWidget();
	auto selectedItems = groupListWidget->selectedItems();
	
	try {
		if (selectedItems.size() != 2) throw (int());
		
		auto selectedItem0Type = itemType(selectedItems[0]),
			 selectedItem1Type = itemType(selectedItems[1]);
		GroupItem *groupItem;
		TypeItem *typeItem;
		
		if ((selectedItem0Type == ItemType::Group || selectedItem0Type == ItemType::RootGroup)
			&& selectedItem1Type == ItemType::Type) {
			groupItem = reinterpret_cast<GroupItem *>(selectedItems[0]);
			typeItem = reinterpret_cast<TypeItem *>(selectedItems[1]);
		} else if ((selectedItem1Type == ItemType::Group || selectedItem1Type == ItemType::RootGroup)
			&& selectedItem0Type == ItemType::Type) {
			groupItem = reinterpret_cast<GroupItem *>(selectedItems[1]);
			typeItem = reinterpret_cast<TypeItem *>(selectedItems[0]);
		} else	// Incorrect selection
			throw (int());
		
		auto groupId = this->groups_.itemsMap.at(groupItem)->id;
		auto typeId = this->types_.itemsMap.at(typeItem)->id;
		
		// Inserting record like "New record 1"
		QString newRecordName, newRecordNamePrefix = QObject::tr("New record ");
		auto newRecordId = invalid_record_id;
		for (size_t i = 1; newRecordId == invalid_record_id; ++i) {
			newRecordName = newRecordNamePrefix + QString::number(i);
			newRecordId = this->kernel_->add_record(groupId, newRecordName.toStdString(), typeId);
		}
		
		this->DATA_loadRecord(newRecordId);
	} catch (...) {
		this->GUI_showWarning(QObject::tr("Error"),
						  QObject::tr("Please, select parent group in list at the left, "
						  			  "then press Ctrl and select record type in the list "
						  			  "and try again."));
	}
}

void CryptoKernelAgent::GUI_addType()
{
	// Inserting type like "New type 1"
	QString newTypeName, newTypeNamePrefix = QObject::tr("New type ");
	auto newTypeId = invalid_type_id;
	for (size_t i = 1; newTypeId == invalid_type_id; ++i) {
		newTypeName = newTypeNamePrefix + QString::number(i);
		newTypeId = this->kernel_->add_type(newTypeName.toStdString());
	}
	
	this->DATA_loadType(newTypeId);
}


void CryptoKernelAgent::GUI_removeSelectedItems()
{
	auto groupListWidget = this->GUI_mainWindow()->groupListWidget();
	auto selectedItems = groupListWidget->selectedItems();
	
	for (auto &selectedItem: selectedItems) {
		auto selectedItemType = itemType(selectedItem);
		
		if (selectedItemType == ItemType::Record) {
			this->DATA_removeRecord(reinterpret_cast<RecordItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Group || selectedItemType == ItemType::RootGroup) {
			this->DATA_removeGroup(reinterpret_cast<GroupItem *>(selectedItem));
		} else if (selectedItemType == ItemType::Type) {
			this->DATA_removeType(reinterpret_cast<TypeItem *>(selectedItem));
		} else if (selectedItemType == ItemType::RootTypeGroup) {	// "Types"
			// Processing all child types (all types)
			for (auto i = selectedItem->childCount() - 1; i >= 0; --i)
				this->DATA_removeType(reinterpret_cast<TypeItem *>(selectedItem->child(i)));
		}	// Else variant is impossible
	}
}


void CryptoKernelAgent::GUI_showWarning(const QString &title, const QString &text)
{
	QMessageBox::information(this->GUI_mainWindow(), title, text,
							 QMessageBox::Ok, QMessageBox::Ok);
}


void CryptoKernelAgent::GUI_showTypeEditDialog()
{
	auto groupListWidget = this->GUI_mainWindow()->groupListWidget();
	auto selectedItems = groupListWidget->selectedItems();
	if (selectedItems.size() != 1 || itemType(selectedItems[0]) != ItemType::Type) {
		this->GUI_showWarning(QObject::tr("Error"),
							  QObject::tr("Please, select a type in the list at the left "
										  "and try again."));
		return;
	}
	auto item = reinterpret_cast<TypeItem *>(selectedItems[0]);
	
	try {
		auto &info = *(this->types_.itemsMap.at(item));
		auto fieldIds = this->kernel_->type_fields(info.id);
		QStringList fieldNames;
		for (auto fieldId: fieldIds)
			fieldNames.append(QString::fromStdString(this->kernel_->type_field_name(info.id, fieldId)));
		
		TypeEditDialog dialog(this->GUI_mainWindow());
		dialog.setName(info.name);
		dialog.setFields(fieldNames);
		
		// Lambda helpers
		auto changeTypeName = [this, item, &dialog](QString newName)
		{
			try {
				auto oldName = this->types_.itemsMap.at(item)->name;
				if (newName == oldName) return;
				
				item->setName(newName);
				if (this->DATA_typeItemNameChanged(item))
					dialog.confirmNameChanges();
			} catch (...) {
				this->GUI_showWarning(QObject::tr("Error"),
									  QObject::tr("Type does not exist."));
			}
		};
		
		auto changeTypeFieldName = [this, item, &info, &dialog, &fieldIds, &fieldNames](int index, QString newName)
		{
			try {
				auto typeId = this->types_.itemsMap.at(item)->id;
				auto oldName = fieldNames.at(index);
				if (newName == oldName) return;
				
				auto fieldId = fieldIds.at(index);
				fieldId = this->kernel_->set_type_field_name(typeId, fieldId, newName.toStdString());
				if (fieldId == invalid_tfield_id)
					this->GUI_showWarning(QObject::tr("Error"),
										  QObject::tr("Can't set name \"%1\" to the type field \"%2\": "
													  "type field with the same name already exists.").arg(newName, oldName));
				else {
					dialog.confirmFieldChanges(index);
					
					if (this->recordContent_.shownRecordId != invalid_record_id
						&& this->kernel_->record_type(this->recordContent_.shownRecordId) == info.id)
						this->GUI_updateRecordContent();
				}
			} catch (...) {	// It's impossible
				this->GUI_showWarning(QObject::tr("Error"),
									  QObject::tr("Type field does not exist."));
			}
		};
		
		auto addTypeField = [this, item, &info, &dialog, &fieldIds, &fieldNames]()
		{
			static const QString newTypeFieldNamePrefix = QObject::tr("New field ");
			
			tfield_id_t newTypeFieldId = invalid_tfield_id;
			for (size_t i = 1; newTypeFieldId == invalid_tfield_id; ++i) {	// Adding type field like "New field 1"
				auto newTypeFieldName = (newTypeFieldNamePrefix + QString::number(i)).toStdString();
				newTypeFieldId = this->kernel_->add_type_field(info.id, newTypeFieldName);
			}
			// Getting inserted name
			auto newTypeFieldName = QString::fromStdString(this->kernel_->type_field_name(info.id, newTypeFieldId));
			fieldIds.push_back(newTypeFieldId);
			fieldNames.append(newTypeFieldName);
			
			// Updating dialog fields
			dialog.addField(newTypeFieldName);
			
			if (this->recordContent_.shownRecordId != invalid_record_id
				&& this->kernel_->record_type(this->recordContent_.shownRecordId) == info.id)
				this->GUI_updateRecordContent();
		};
		
		QObject::connect(&dialog, &TypeEditDialog::nameChanged,  changeTypeName);
		QObject::connect(&dialog, &TypeEditDialog::fieldChanged, changeTypeFieldName);
		QObject::connect(&dialog, &TypeEditDialog::fieldAdded,   addTypeField);
		dialog.exec();
	} catch (...) {}
}
