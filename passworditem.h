// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDITEM_H
#define PASSWORDITEM_H

#include <QTreeWidget>

#include <QString>
#include <QList>
#include <QPair>

#include <map>

#include "types.h"
#include "abstractitem.h"
#include "groupitem.h"

class PasswordItem: public AbstractItem
{
public:
	PasswordItem(id_t id, const QString &passwordName, GroupItem *parent = nullptr);
	PasswordItem(id_t id, const QString &passwordName, QTreeWidget *parent = nullptr);
private:
	QList<QPair<tfield_id_t, QString>> fields_;	// Fields list: (FIELD_ID, FIELD_STRING)
};

#endif // PASSWORDITEM_H
