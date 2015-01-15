// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QTreeWidget>

#include <QString>

#include "abstractitem.h"

class GroupItem: public AbstractItem
{
public:
	GroupItem(const QString &groupName, GroupItem *parent = nullptr);
	GroupItem(const QString &groupName, QTreeWidget *parent);
};

#endif // GROUPITEM_H
