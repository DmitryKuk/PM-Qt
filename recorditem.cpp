// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recorditem.h"

RecordItem::RecordItem(const QString &recordName,
					   GroupItem *parent):
	AbstractItem(recordName, parent)
{}

RecordItem::RecordItem(const QString &recordName,
					   QTreeWidget *parent):
	AbstractItem(recordName, parent)
{}
