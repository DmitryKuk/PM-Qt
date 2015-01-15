// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(const QString &groupName,
					 GroupItem *parent):
	AbstractItem(groupName, parent)
{
	setItemType(this, ::ItemType::Group);
}

GroupItem::GroupItem(const QString &groupName,
					 QTreeWidget *parent):
	AbstractItem(groupName, parent)
{
	setItemType(this, ::ItemType::Group);
}
