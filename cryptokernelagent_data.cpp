// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"
#include <queue>


QString CryptoKernelAgent::typeName(RecordItem *recordItem) const
{
	try {
		return this->records_.itemsMap.at(recordItem)->name;
	} catch (...) {}
	return QString();
}

QString CryptoKernelAgent::parentGroupName(RecordItem *recordItem) const
{
	try {
		auto parentGroupId = this->kernel_.record_parent_group(this->records_.itemsMap.at(recordItem)->id);
		return this->groups_.idsMap.at(parentGroupId)->name;
	} catch (...) {}
	return QString();
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


void CryptoKernelAgent::Groups::erase(group_id_t id)
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


void CryptoKernelAgent::addRootGroup()
{
	auto rootGroupId = this->kernel_.root_group_id();
	if (rootGroupId == invalid_group_id)	// Creating root group, if it does not exist
		rootGroupId = this->kernel_.add_root_group();
	
	QString rootGroupName = QObject::tr("Data");
	auto rootGroupItem = new GroupItem(rootGroupName);
	setItemType(rootGroupItem, ItemType::RootGroup);
	
	auto groupListWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	groupListWidget->insertTopLevelItem(0, rootGroupItem);
	rootGroupItem->setExpanded(true);
	
	// Updating agent's cache
	this->groups_.add({ .id = rootGroupId,
						.item = rootGroupItem,
						.name = rootGroupName });
}

void CryptoKernelAgent::removeRootGroup()
{
	// Cleaning records in record list
	for (auto &info: this->records_.list)
		delete info.recordListItem;
	this->records_.clear();
	
	auto rootGroupId = this->kernel_.root_group_id();
	try {
		auto rootGroupItem = this->groups_.idsMap.at(rootGroupId)->item;
		delete rootGroupItem;	// Clears all record and group items
	} catch (...) {}
	this->groups_.clear();
	
	this->kernel_.remove_group(rootGroupId);	// Clears all records and groups in kernel
	this->addRootGroup();	// Adds empty root group and "Data" to the left panel
}


void CryptoKernelAgent::loadGroups()
{
	this->addRootGroup(); // Adding root group
	
	std::queue<group_id_t> groupIdQueue;	// Queue for BFS
	groupIdQueue.push(this->kernel_.root_group_id());
	
	while (!groupIdQueue.empty()) {	// Groups
		auto currentGroupId = groupIdQueue.front();
		groupIdQueue.pop();
		
		auto currentGroupItem = this->groups_.idsMap[currentGroupId]->item;
		for (auto childGroupId: this->kernel_.groups(currentGroupId)) {
			groupIdQueue.push(childGroupId);
			
			QString childGroupName = QString::fromStdString(this->kernel_.group_name(childGroupId));
			auto childGroupItem = new GroupItem(childGroupName, currentGroupItem);
			
			// Updating agent's cache
			this->groups_.add({ .id = childGroupId,
								.item = childGroupItem,
								.name = childGroupName });
		}
	}
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


void CryptoKernelAgent::Records::erase(record_id_t id)
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


void CryptoKernelAgent::loadRecords()
{
	auto recordListWidget = this->mainWindow()->mainWidget()->recordListWidget();
	
	for (auto recordId: this->kernel_.records()) {
		auto parentGroupId = this->kernel_.record_parent_group(recordId);
		auto parentGroupItem = this->groups_.idsMap[parentGroupId]->item;
		
		// Adding data into groupsListWidget
		QString recordName = QString::fromStdString(this->kernel_.record_name(recordId));
		auto recordItem = new RecordItem(recordName, parentGroupItem);
		
		
		// Adding data into recordListWidget
		auto recordListItem = new QTreeWidgetItem(recordListWidget);
		recordListItem->setText(RecordFieldPos::Name,
								recordName);
		
		auto recordTypeId = this->kernel_.record_type(recordId);
		recordListItem->setText(RecordFieldPos::TypeName,
								this->types_.idsMap[recordTypeId]->name);
		
		auto recordParentGroupId = this->kernel_.record_parent_group(recordId);
		recordListItem->setText(RecordFieldPos::ParentGroup,
								this->groups_.idsMap[recordParentGroupId]->name);
		
		// Updating agent's cache
		this->records_.add({ .id = recordId,
							 .item = recordItem,
							 .recordListItem = recordListItem,
							 .name = recordName });
	}
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


void CryptoKernelAgent::Types::erase(record_id_t id)
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


void CryptoKernelAgent::RecordContent::clear()
{
	this->shownRecordId = invalid_record_id;
	this->shownFieldIds.clear();
}


void CryptoKernelAgent::loadTypes()
{
	// Creating root type group
	auto rootGroupWidget = this->mainWindow()->leftPanelWidget()->groupListWidget();
	this->types_.rootGroup = new GroupItem(QObject::tr("Types"), rootGroupWidget);
	setItemType(this->types_.rootGroup, ItemType::RootTypeGroup);
	this->types_.rootGroup->setExpanded(true);
	
	// Loading types
	for (auto typeId: this->kernel_.types()) {
		QString typeName = QString::fromStdString(this->kernel_.type_name(typeId));
		auto typeItem = new TypeItem(typeName, this->types_.rootGroup);
		
		// Updating agent's cache
		this->types_.add({ .id = typeId,
						   .item = typeItem,
						   .name = typeName });
	}
}


void CryptoKernelAgent::loadData()
{
	this->loadGroups();		// Adding groups (must be before loadRecords()!)
	this->loadTypes();		// Adding types (must be before loadRecords()!)
	this->loadRecords();	// Adding records
}
