// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "cryptokernelagent.h"

#include <iostream>
#include <fstream>
#include <stack>


CryptoKernelAgent::CryptoKernelAgent():
	mainWindow_(new MainWindow(this)),
	warningWindow_(new WarningWindow(nullptr))
{
	std::ifstream f("test_input.txt");
	if (!this->kernel_.read(f))
		std::cerr << "ERROR: CryptoKernelAgent constructor: Kernel can't read data!" << std::endl;
}

CryptoKernelAgent::~CryptoKernelAgent()
{
	delete this->warningWindow_;
	delete this->mainWindow_;
	
	std::ofstream f("test_input.txt");
	if (!this->kernel_.write(f))
		std::cerr << "ERROR: CryptoKernelAgent destructor: Kernel can't write data!" << std::endl;
}


void CryptoKernelAgent::run()
{
	this->loadData();
	this->mainWindow()->show();
	this->mainWindow()->mainWidget()->recordContentWidget()->hide();
}


void CryptoKernelAgent::removeRecord(RecordItem *item)
{
	try {
		auto &info = *(this->records_.itemsMap.at(item));
		delete info.recordListItem;	// Removing items
		delete item;
		this->kernel_.remove_record(info.id);	// Removing record from kernel
		this->records_.erase(item);	// Cleaning cache
	} catch (...) {}
}


void CryptoKernelAgent::removeGroup(GroupItem *item)
{
	try {
		auto rootGroupId = this->groups_.idsMap.at(this->kernel_.root_group_id())->id;
		auto id = this->groups_.itemsMap.at(item)->id;
		
		if (id == rootGroupId)
			this->removeRootGroup();
		else {	
			for (auto i = item->childCount() - 1; i >= 0; --i) {
				auto childItem = item->child(i);
				if (itemType(childItem) == ItemType::Record)
					this->removeRecord(reinterpret_cast<RecordItem *>(childItem));
				else
					this->removeGroup(reinterpret_cast<GroupItem *>(childItem));
			}
			delete item;	// Qt cleans all children
			this->kernel_.remove_group(id);
			this->groups_.erase(item);	// Cleaning cache
		}
	} catch (...) {}
}


void CryptoKernelAgent::removeType(TypeItem *item)
{
	try {
		auto id = this->types_.itemsMap.at(item)->id;
		for (auto recordId: this->kernel_.records_of_type(id))
			this->removeRecord(this->records_.idsMap.at(recordId)->item);
		
		this->kernel_.remove_type(id);
		delete item;
		this->types_.erase(item);
	} catch (...) {}
}


void CryptoKernelAgent::showRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->records_.itemsMap.at(recordItem)->recordListItem;
		recordListItem->setHidden(false);
		this->records_.shownRecordListItems.insert(recordListItem);
	} catch (...) {}
}

void CryptoKernelAgent::hideRecordInList(RecordItem *recordItem)
{
	try {
		auto recordListItem = this->records_.itemsMap.at(recordItem)->recordListItem;
		recordListItem->setHidden(true);
		this->records_.shownRecordListItems.erase(recordListItem);
	} catch (...) {}
}
