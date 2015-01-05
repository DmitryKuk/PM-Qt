// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "abstractitem.h"

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


QString AbstractItem::name() const
{
	return this->text(0);
}

void AbstractItem::setName(const QString &newName)
{
	this->setText(0, newName);
}
