// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TYPEFIELDITEM_H
#define TYPEFIELDITEM_H

#include <QString>

#include "abstractitem.h"
#include "typeitem.h"

class TypeFieldItem: public AbstractItem
{
public:
	TypeFieldItem(const QString &typeFieldName, TypeItem *parent = nullptr);
	TypeFieldItem(const QString &typeFieldName, QTreeWidget *parent);
};

#endif // TYPEFIELDITEM_H
