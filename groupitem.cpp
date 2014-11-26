// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(id_t id,
					 const QString &groupName,
					 AbstractItem *parent):
	AbstractItem(id, groupName, parent)
{}

GroupItem::GroupItem(id_t id,
					 const QString &groupName,
					 QWidget *parent):
	AbstractItem(id, groupName, parent)
{}
