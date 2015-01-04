// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "abstractitem.h"


// Item types
const int
	ItemType::Abstract = 0,
	ItemType::Group = 10,
	ItemType::Record = 20,
	ItemType::Type = 30;


AbstractItem::AbstractItem(const QString &itemName,
						   AbstractItem *parent):
	QTreeWidgetItem(parent)
{
	this->setText(0, itemName);
}

AbstractItem::AbstractItem(const QString &itemName,
						   QTreeWidget *parent):
	QTreeWidgetItem(parent)
{
	this->setText(0, itemName);
}

AbstractItem::~AbstractItem()
{}


int AbstractItem::itemType() const
{ return ::ItemType::Abstract; }
