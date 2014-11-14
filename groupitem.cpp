// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(const id_t &id,
					 const QString &groupName,
					 QTreeWidget *parent):
	QTreeWidgetItem(parent, QTreeWidgetItem::UserType),
	id_(id)
{
	this->setName(groupName);
}


GroupItem::GroupItem(const id_t &id,
					 const QString &groupName,
					 QTreeWidgetItem *parent):
	QTreeWidgetItem(parent, QTreeWidgetItem::UserType),
	id_(id)
{
	this->setName(groupName);
}


const id_t & GroupItem::id() const
{
	return this->id_;
}


QString GroupItem::name() const
{
	return this->text(0);
}

void GroupItem::setName(const QString &groupName)
{
	this->setText(0, groupName);
}
