// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "groupitem.h"

GroupItem::GroupItem(const id_t &id, QTreeWidget *parent):
	QTreeWidgetItem(parent, QTreeWidgetItem::UserType),
	id_(id)
{}


const id_t & GroupItem::id() const
{
	return this->id_;
}


QString GroupItem::name() const
{
	return this->text(0);
}

void GroupItem::setName(const QString &new_name)
{
	this->setText(0, new_name);
}
