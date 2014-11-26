// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QString>

#include "types.h"
#include "abstractitem.h"

class GroupItem: public AbstractItem
{
public:
	GroupItem(id_t id, const QString &groupName, AbstractItem *parent = nullptr);
	GroupItem(id_t id, const QString &groupName, QWidget *parent = nullptr);
};

#endif // GROUPITEM_H
