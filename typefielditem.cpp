// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "typefielditem.h"

TypeFieldItem::TypeFieldItem(const QString &typeFieldName,
							 TypeItem *parent):
	AbstractItem(typeFieldName, parent)
{
	setItemType(this, ::ItemType::TypeField);
	this->setFlags(this->flags() | Qt::ItemIsEditable);
}

TypeFieldItem::TypeFieldItem(const QString &typeFieldName,
							 QTreeWidget *parent):
	AbstractItem(typeFieldName, parent)
{
	setItemType(this, ::ItemType::TypeField);
	this->setFlags(this->flags() | Qt::ItemIsEditable);
}
