// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef RECORDITEM_H
#define RECORDITEM_H

#include <QTreeWidget>

#include <QString>

#include "abstractitem.h"
#include "groupitem.h"

class RecordItem: public AbstractItem
{
public:
	RecordItem(const QString &recordName, GroupItem *parent = nullptr);
	RecordItem(const QString &recordName, QTreeWidget *parent);
};

#endif // RECORDITEM_H
