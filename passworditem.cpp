// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passworditem.h"

PasswordItem::PasswordItem(const id_t &id,
						   const QStringList &passwordProperties,
						   QTreeWidget *parent):
	QTreeWidgetItem(parent, passwordProperties, QTreeWidgetItem::UserType),
	id_(id)
{}

const id_t & PasswordItem::id() const
{
	return this->id_;
}
