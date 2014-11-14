// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QtWidgets>

#include "types.h"

class GroupItem: public QTreeWidgetItem
{
public:
	GroupItem(const id_t &id, const QString &groupName, QTreeWidget *parent = nullptr);
	GroupItem(const id_t &id, const QString &groupName, QTreeWidgetItem *parent = nullptr);
	
	virtual const id_t & id() const;
	
	virtual QString name() const;
	virtual void setName(const QString &groupName);
private:
	id_t id_;
};

#endif // GROUPITEM_H
