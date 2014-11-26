// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "abstractitem.h"

AbstractItem::AbstractItem(id_t id,
						   const QString &itemName,
						   AbstractItem *parent):
	QTreeWidgetItem(parent),
	id_(id)
{
	this->setText(0, itemName);
}

AbstractItem::AbstractItem(id_t id,
						   const QString &itemName,
						   QTreeWidget *parent):
	QTreeWidgetItem(parent),
	id_(id)
{
	this->setText(0, itemName);
}

AbstractItem::~AbstractItem()
{}
