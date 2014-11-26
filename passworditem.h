// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDITEM_H
#define PASSWORDITEM_H

#include <QString>
#include <QList>
#include <QPair>

#include "types.h"
#include "abstractitem.h"
#include "groupitem.h"

class PasswordItem: public AbstractItem
{
public:
	PasswordItem(id_t id, const QString &passwordName, GroupItem *parent = nullptr);
	PasswordItem(id_t id, const QString &passwordName, QWidget *parent = nullptr);
private:
	QList<QPair<field_id_t, QString>> fields_;	// Fields list: (FIELD_ID, FIELD_STRING)
};

#endif // PASSWORDITEM_H
