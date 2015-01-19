// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "abstractitem.h"

// Constants for identification of selected elements in groupListWidget
// Set one of these with setData(0, Qt::UserRole, oneOfThese)
const int
	ItemType::Abstract		=  5,
	ItemType::Record		= 10,
	ItemType::Group			= 20,
	ItemType::Type			= 30,
	ItemType::RootGroup		= 40,
	ItemType::RootTypeGroup	= 50,
	ItemType::TypeField		= 60;


AbstractItem::AbstractItem(const QString &itemName,
						   AbstractItem *parent):
	QTreeWidgetItem(parent)
{
	this->setText(0, itemName);
	setItemType(this, ::ItemType::Abstract);
}

AbstractItem::AbstractItem(const QString &itemName,
						   QTreeWidget *parent):
	QTreeWidgetItem(parent)
{
	this->setText(0, itemName);
	setItemType(this, ::ItemType::Abstract);
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
