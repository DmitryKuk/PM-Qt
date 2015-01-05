// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(const QString &groupName,
					 GroupItem *parent):
	AbstractItem(groupName, parent)
{}

GroupItem::GroupItem(const QString &groupName,
					 QTreeWidget *parent):
	AbstractItem(groupName, parent)
{}
