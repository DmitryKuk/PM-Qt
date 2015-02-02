// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(const QString &groupName,
					 GroupItem *parent,
					 bool isRootGroup):
	AbstractItem(groupName, parent)
{
	if (isRootGroup)
		setItemType(this, ::ItemType::RootGroup);
	else {
		setItemType(this, ::ItemType::Group);
		this->setFlags(this->flags() | Qt::ItemIsEditable);
	}
	this->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
}

GroupItem::GroupItem(const QString &groupName,
					 QTreeWidget *parent,
					 bool isRootGroup):
	AbstractItem(groupName, parent)
{
	if (isRootGroup)
		setItemType(this, ::ItemType::RootGroup);
	else {
		setItemType(this, ::ItemType::Group);
		this->setFlags(this->flags() | Qt::ItemIsEditable);
	}
	this->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
}
