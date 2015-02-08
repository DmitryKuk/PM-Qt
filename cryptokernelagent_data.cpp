// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <queue>


QString CryptoKernelAgent::DATA_typeName(RecordItem *recordItem) const
{
	try {
		return this->records_.itemsMap.at(recordItem)->name;
	} catch (...) {}
	return QString();
}

QString CryptoKernelAgent::DATA_parentGroupName(RecordItem *recordItem) const
{
	try {
		auto parentGroupId = this->kernel_->record_parent_group(this->records_.itemsMap.at(recordItem)->id);
		return this->groups_.idsMap.at(parentGroupId)->name;
	} catch (...) {}
	return QString();
}


void CryptoKernelAgent::DATA_showRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->records_.itemsMap.at(recordItem)->recordListItem;
		recordListItem->setHidden(false);
		this->records_.shownRecordListItems.insert(recordListItem);
	} catch (...) {}
}

void CryptoKernelAgent::DATA_hideRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->records_.itemsMap.at(recordItem)->recordListItem;
		recordListItem->setHidden(true);
		this->records_.shownRecordListItems.erase(recordListItem);
	} catch (...) {}
}


void CryptoKernelAgent::DATA_removeRecord(RecordItem *item)
{
	try {
		auto &info = *(this->records_.itemsMap.at(item));
		delete info.recordListItem;	// Removing items
		delete item;
		this->kernel_->remove_record(info.id);	// Removing record from kernel
		this->records_.erase(item);	// Cleaning cache
	} catch (...) {}
}


void CryptoKernelAgent::DATA_removeGroup(GroupItem *item)
{
	try {
		auto rootGroupId = this->groups_.idsMap.at(this->kernel_->root_group_id())->id;
		auto id = this->groups_.itemsMap.at(item)->id;
		
		if (id == rootGroupId)
			this->DATA_removeRootGroup();
		else {	
			for (auto i = item->childCount() - 1; i >= 0; --i) {
				auto childItem = item->child(i);
				if (itemType(childItem) == ItemType::Record)
					this->DATA_removeRecord(reinterpret_cast<RecordItem *>(childItem));
				else
					this->DATA_removeGroup(reinterpret_cast<GroupItem *>(childItem));
			}
			delete item;	// Qt cleans all children
			this->kernel_->remove_group(id);
			this->groups_.erase(item);	// Cleaning cache
		}
	} catch (...) {}
}


void CryptoKernelAgent::DATA_removeType(TypeItem *item)
{
	try {
		auto id = this->types_.itemsMap.at(item)->id;
		for (auto recordId: this->kernel_->records_of_type(id))
			this->DATA_removeRecord(this->records_.idsMap.at(recordId)->item);
		
		this->kernel_->remove_type(id);
		delete item;
		this->types_.erase(item);
	} catch (...) {}
}


// Groups
bool CryptoKernelAgent::Groups::add(const CryptoKernelAgent::Groups::GroupInfo &info)
{
	auto it1 = this->idsMap.find(info.id);
	auto it2 = this->itemsMap.find(info.item);
	if (it1 != this->idsMap.end()
		|| it2 != this->itemsMap.end())
		return false;
	
	auto it = this->list.insert(this->list.end(), info);
	this->idsMap[it->id] = it;
	this->itemsMap[it->item] = it;
	return true;
}


void CryptoKernelAgent::Groups::erase(types::group_id id)
{
	auto it1 = this->idsMap.find(id);
	if (it1 == this->idsMap.end()) return;
	
	auto it = it1->second;
	this->idsMap.erase(it1);
	this->itemsMap.erase(it->item);
	this->list.erase(it);
}

void CryptoKernelAgent::Groups::erase(GroupItem *item)
{
	auto it2 = this->itemsMap.find(item);
	if (it2 == this->itemsMap.end()) return;
	
	auto it = it2->second;
	this->idsMap.erase(it->id);
	this->itemsMap.erase(it2);
	this->list.erase(it);
}


void CryptoKernelAgent::Groups::clear()
{
	this->list.clear();
	this->idsMap.clear();
	this->itemsMap.clear();
}


void CryptoKernelAgent::DATA_addRootGroup()
{
	auto rootGroupId = this->kernel_->root_group_id();
	if (rootGroupId.is_invalid())	// Creating root group, if it does not exist
		rootGroupId = this->kernel_->add_root_group();
	
	QString rootGroupName = QObject::tr("Data");
	auto rootGroupItem = new GroupItem(rootGroupName, static_cast<GroupListWidget *>(nullptr), true);
	
	auto groupListWidget = this->GUI_mainWindow()->leftPanelWidget()->groupListWidget();
	groupListWidget->insertTopLevelItem(0, rootGroupItem);
	rootGroupItem->setExpanded(true);
	
	// Updating agent's cache
	this->groups_.add({ .id = rootGroupId,
						.item = rootGroupItem,
						.name = rootGroupName });
}

void CryptoKernelAgent::DATA_removeRootGroup()
{
	// Cleaning records in record list
	for (auto &info: this->records_.list)
		delete info.recordListItem;
	this->records_.clear();
	
	auto rootGroupId = this->kernel_->root_group_id();
	try {
		auto rootGroupItem = this->groups_.idsMap.at(rootGroupId)->item;
		delete rootGroupItem;	// Clears all record and group items
	} catch (...) {}
	this->groups_.clear();
	
	this->kernel_->remove_group(rootGroupId);	// Clears all records and groups in kernel
	this->DATA_addRootGroup();	// Adds empty root group and "Data" to the left panel
}


bool CryptoKernelAgent::DATA_loadGroup(types::group_id id)	// Do NOT load ROOT group, use DATA_addRootGroup()!
{
	try {
		auto parentGroupId = this->kernel_->group_parent_group(id);
		if (parentGroupId.is_invalid()) throw (int());
		auto parentGroupItem = this->groups_.idsMap.at(parentGroupId)->item;
		
		Groups::GroupInfo info;
		info.id = id;
		info.name = QString::fromStdString(this->kernel_->group_name(id));
		if (info.name.isEmpty()) throw (int());
		info.item = new GroupItem(info.name, parentGroupItem);
		
		this->groups_.add(info);	// Updating agent's cache
	} catch (...) {
		return false;
	}
	return true;
}

void CryptoKernelAgent::DATA_loadGroups()
{
	std::queue<types::group_id> groupIdQueue;	// Queue for BFS
	
	// Adding root group
	{
		this->DATA_addRootGroup();
		auto rootGroupId = this->kernel_->root_group_id();
		for (auto childGroupId: this->kernel_->groups(rootGroupId))	// Load its child groups
			groupIdQueue.push(childGroupId);
	}
	
	while (!groupIdQueue.empty()) {	// Groups
		auto currentGroupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		if (this->DATA_loadGroup(currentGroupId)) {	// If current group loaded successfully
			for (auto childGroupId: this->kernel_->groups(currentGroupId))	// Load its child groups
				groupIdQueue.push(childGroupId);
		}
	}
}


bool CryptoKernelAgent::DATA_groupItemNameChanged(GroupItem *item)
{
	try {
		auto &info = *(this->groups_.itemsMap.at(item));
		
		auto newName = item->name();
		if (newName == info.name) return true;
		
		auto id = this->kernel_->set_group_name(info.id, newName.toStdString());
		if (id.is_invalid()) {	// Name not changed
			this->GUI_showWarning(QObject::tr("Error"),
								  QObject::tr("Can't set name \"%1\" to the group \"%2\": "
											  "group with the same name already exists.").arg(newName, info.name));
			item->setName(info.name);
			return false;
		} else {	// Name changed
			info.name = newName;
			
			// Updating child records parent group names
			for (auto recordId: this->kernel_->records(info.id))
				try {
					auto &recordInfo = *(this->records_.idsMap.at(recordId));
					recordInfo.recordListItem->setText(RecordFieldPos::ParentGroup, info.name);
					
					if (this->GUI_mainWindow()->recordContentWidget()->recordId() == recordInfo.id)	// Updating record content
						this->GUI_updateRecordContent();
				} catch (...) {}
		}
	} catch (...) {
		return false;
	}
	return true;
}


// Records
bool CryptoKernelAgent::Records::add(const CryptoKernelAgent::Records::RecordInfo &info)
{
	auto it1 = this->idsMap.find(info.id);
	auto it2 = this->itemsMap.find(info.item);
	auto it3 = this->recordListItemsMap.find(info.recordListItem);
	if (it1 != this->idsMap.end()
		|| it2 != this->itemsMap.end()
		|| it3 != this->recordListItemsMap.end())
		return false;
	
	auto it = this->list.insert(this->list.end(), info);
	this->idsMap[it->id] = it;
	this->itemsMap[it->item] = it;
	this->recordListItemsMap[it->recordListItem] = it;
	this->shownRecordListItems.insert(it->recordListItem);
	return true;
}


void CryptoKernelAgent::Records::erase(types::record_id id)
{
	auto it1 = this->idsMap.find(id);
	if (it1 == this->idsMap.end()) return;
	
	auto it = it1->second;
	this->idsMap.erase(it1);
	this->itemsMap.erase(it->item);
	this->recordListItemsMap.erase(it->recordListItem);
	this->shownRecordListItems.erase(it->recordListItem);
	this->list.erase(it);
}

void CryptoKernelAgent::Records::erase(RecordItem *item)
{
	auto it2 = this->itemsMap.find(item);
	if (it2 == this->itemsMap.end()) return;
	
	auto it = it2->second;
	this->idsMap.erase(it->id);
	this->itemsMap.erase(it2);
	this->recordListItemsMap.erase(it->recordListItem);
	this->shownRecordListItems.erase(it->recordListItem);
	this->list.erase(it);
}

void CryptoKernelAgent::Records::erase(QTreeWidgetItem *recordListItem)
{
	auto it3 = this->recordListItemsMap.find(recordListItem);
	if (it3 == this->recordListItemsMap.end()) return;
	
	auto it = it3->second;
	this->idsMap.erase(it->id);
	this->itemsMap.erase(it->item);
	this->recordListItemsMap.erase(it3);
	this->shownRecordListItems.erase(recordListItem);
	this->list.erase(it);
}


void CryptoKernelAgent::Records::clear()
{
	this->list.clear();
	this->idsMap.clear();
	this->itemsMap.clear();
	this->shownRecordListItems.clear();
}


bool CryptoKernelAgent::DATA_loadRecord(types::record_id id)
{
	try {
		auto recordListWidget = this->GUI_mainWindow()->mainWidget()->recordListWidget();
		
		auto parentGroupId = this->kernel_->record_parent_group(id);
		if (parentGroupId.is_invalid()) throw (int());
		
		auto parentGroupItem = this->groups_.idsMap.at(parentGroupId)->item;
		
		// Adding data into groupsListWidget
		Records::RecordInfo info;
		info.id = id;
		info.name = QString::fromStdString(this->kernel_->record_name(id));
		info.item = new RecordItem(info.name, parentGroupItem);
		
		// Adding data into recordListWidget
		info.recordListItem = new QTreeWidgetItem(recordListWidget);
		auto typeId = this->kernel_->record_type(id);
		auto recordParentGroupId = this->kernel_->record_parent_group(id);
		
		// Filling item's data
		info.recordListItem->setText(RecordFieldPos::Name,			info.name);
		info.recordListItem->setText(RecordFieldPos::TypeName,		this->types_.idsMap.at(typeId)->name);
		info.recordListItem->setText(RecordFieldPos::ParentGroup,	this->groups_.idsMap.at(recordParentGroupId)->name);
		
		// Updating agent's cache
		this->records_.add(info);
	} catch (...) {
		return false;
	}
	return true;
}


void CryptoKernelAgent::DATA_loadRecords()
{
	for (auto id: this->kernel_->records())
		this->DATA_loadRecord(id);
}


void CryptoKernelAgent::DATA_addRecordField()
{
	static const QString newRecordName = QObject::tr("New field");
	
	auto recordId = this->GUI_mainWindow()->recordContentWidget()->recordId();
	if (recordId.is_invalid()) return;
	
	auto recordFieldId = this->kernel_->add_field(recordId, types::tfield_id::invalid(), newRecordName.toStdString());
	if (recordFieldId.is_invalid())
		this->GUI_showWarning(QObject::tr("Error"),
							  QObject::tr("Unknown error."));
	else {
		auto recordFieldData = QString::fromStdString(this->kernel_->field_data(recordId, recordFieldId));
		auto recordFieldTypeId = this->kernel_->field_type(recordId, recordFieldId);
		this->GUI_mainWindow()->recordContentWidget()->addField(recordFieldId, recordFieldTypeId, recordFieldData);
	}
}


void CryptoKernelAgent::DATA_setRecordFieldType(types::rfield_id fieldId, types::tfield_id typeFieldId)
{
	auto recordId = this->GUI_mainWindow()->recordContentWidget()->recordId();
	if (recordId.is_invalid()) return;
	this->kernel_->set_field_type(recordId, fieldId, typeFieldId);
}


bool CryptoKernelAgent::DATA_recordItemNameChanged(RecordItem *item)
{
	try {
		auto &info = *(this->records_.itemsMap.at(item));
		
		auto newName = item->name();
		if (newName == info.name) return true;
		
		auto id = this->kernel_->set_record_name(info.id, newName.toStdString());
		if (id.is_invalid()) {	// Name not changed
			this->GUI_showWarning(QObject::tr("Error"),
								  QObject::tr("Can't set name \"%1\" to the record \"%2\": "
											  "record with the same name already exists.").arg(newName, info.name));
			item->setName(info.name);
			return false;
		} else {	// Name changed
			info.name = newName;
			info.recordListItem->setText(RecordFieldPos::Name, info.name);
			
			if (this->GUI_mainWindow()->recordContentWidget()->recordId() == info.id)
				this->GUI_updateRecordContent();
		}
	} catch (...) {
		return false;
	}
	return true;
}


// Types
bool CryptoKernelAgent::Types::add(const CryptoKernelAgent::Types::TypeInfo &info)
{
	auto it1 = this->idsMap.find(info.id);
	auto it2 = this->itemsMap.find(info.item);
	if (it1 != this->idsMap.end()
		|| it2 != this->itemsMap.end())
		return false;
	
	auto it = this->list.insert(this->list.end(), info);
	this->idsMap[it->id] = it;
	this->itemsMap[it->item] = it;
	return true;
}


void CryptoKernelAgent::Types::erase(types::type_id id)
{
	auto it1 = this->idsMap.find(id);
	if (it1 == this->idsMap.end()) return;
	
	auto it = it1->second;
	this->idsMap.erase(it1);
	this->itemsMap.erase(it->item);
	this->list.erase(it);
}

void CryptoKernelAgent::Types::erase(TypeItem *item)
{
	auto it2 = this->itemsMap.find(item);
	if (it2 == this->itemsMap.end()) return;
	
	auto it = it2->second;
	this->idsMap.erase(it->id);
	this->itemsMap.erase(it2);
	this->list.erase(it);
}


void CryptoKernelAgent::Types::clear()
{
	this->list.clear();
	this->idsMap.clear();
	this->itemsMap.clear();
}


bool CryptoKernelAgent::DATA_loadType(types::type_id id)
{
	Types::TypeInfo info;
	info.id = id;
	info.name = QString::fromStdString(this->kernel_->type_name(id));
	if (info.name == "") return false;	// Type not found
	info.item = new TypeItem(info.name, this->types_.rootGroup);
	this->types_.add(info);	// Updating agent's cache
	return true;
}

void CryptoKernelAgent::DATA_loadTypes()
{
	// Creating root type group
	auto rootGroupWidget = this->GUI_mainWindow()->leftPanelWidget()->groupListWidget();
	this->types_.rootGroup = new GroupItem(QObject::tr("Types"), rootGroupWidget, true);
	setItemType(this->types_.rootGroup, ItemType::RootTypeGroup);
	this->types_.rootGroup->setExpanded(true);
	
	// Loading types
	for (auto id: this->kernel_->types())
		this->DATA_loadType(id);
}


void CryptoKernelAgent::DATA_removeTypeField(TypeItem *item, types::tfield_id fieldId)
{
	try {
		auto typeId = this->types_.itemsMap.at(item)->id;
		this->kernel_->remove_type_field(typeId, fieldId);
		
		if (this->GUI_mainWindow()->recordContentWidget()->recordId().is_valid()
			&& this->kernel_->record_type(this->GUI_mainWindow()->recordContentWidget()->recordId()) == typeId)
			this->GUI_updateRecordContent();
	} catch (...) {}
}


bool CryptoKernelAgent::DATA_typeItemNameChanged(TypeItem *item)
{
	try {
		auto &info = *(this->types_.itemsMap.at(item));
		
		auto newName = item->name();
		if (newName == info.name) return true;
		
		auto id = this->kernel_->set_type_name(info.id, newName.toStdString());
		if (id.is_invalid()) {	// Name not changed
			item->setName(info.name);
			this->GUI_showWarning(QObject::tr("Error"),
								  QObject::tr("Can't set name \"%1\" to the type \"%2\": "
											  "type with the same name already exists.").arg(newName, info.name));
			return false;
		} else {						// Name changed successfully
			info.name = newName;
			
			// Updating child records type names
			for (auto recordId: this->kernel_->records_of_type(info.id))
				try {
					auto &recordInfo = *(this->records_.idsMap.at(recordId));
					recordInfo.recordListItem->setText(RecordFieldPos::TypeName, info.name);
					
					if (this->GUI_mainWindow()->recordContentWidget()->recordId() == recordInfo.id)	// Updating record content
						this->GUI_updateRecordContent();
				} catch (...) {}
		}
	} catch (...) {
		return false;
	}
	return true;
}


void CryptoKernelAgent::DATA_loadData()
{
	this->DATA_loadGroups();	// Adding groups (must be before loadRecords()!)
	this->DATA_loadTypes();		// Adding types (must be before loadRecords()!)
	this->DATA_loadRecords();	// Adding records
}
