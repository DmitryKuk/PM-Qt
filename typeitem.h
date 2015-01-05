// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TYPEITEM_H
#define TYPEITEM_H

#include <QTreeWidget>

#include <QString>

#include "abstractitem.h"
#include "groupitem.h"

class TypeItem: public AbstractItem
{
public:
	TypeItem(const QString &typeName, GroupItem *parent = nullptr);
	TypeItem(const QString &typeName, QTreeWidget *parent = nullptr);
};

#endif // TYPEITEM_H
