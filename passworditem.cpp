// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passworditem.h"

PasswordItem::PasswordItem(id_t id,
						   const QString &passwordName,
						   GroupItem *parent):
	AbstractItem(id, passwordName, parent)
{}

PasswordItem::PasswordItem(id_t id,
						   const QString &passwordName,
						   QTreeWidget *parent):
	AbstractItem(id, passwordName, parent)
{}
