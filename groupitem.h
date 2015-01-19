// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QString>

#include "abstractitem.h"

class GroupItem: public AbstractItem
{
public:
	GroupItem(const QString &groupName, GroupItem *parent = nullptr, bool isRootGroup = false);
	GroupItem(const QString &groupName, QTreeWidget *parent, bool isRootGroup = false);
};

#endif // GROUPITEM_H
