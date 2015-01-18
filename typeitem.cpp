// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "typeitem.h"

TypeItem::TypeItem(const QString &typeName,
				   GroupItem *parent):
	AbstractItem(typeName, parent)
{
	setItemType(this, ::ItemType::Type);
	this->setFlags(this->flags() | Qt::ItemIsEditable);
}

TypeItem::TypeItem(const QString &typeName,
				   QTreeWidget *parent):
	AbstractItem(typeName, parent)
{
	setItemType(this, ::ItemType::Type);
	this->setFlags(this->flags() | Qt::ItemIsEditable);
}
