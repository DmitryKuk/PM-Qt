// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QString>

namespace ItemType {

extern const int
	Abstract,	// = 0
	Group,		// = 10
	Record,		// = 20
	Type;		// = 30

};


class AbstractItem: public QTreeWidgetItem
{
public:
	AbstractItem(const QString &itemName, AbstractItem *parent = nullptr);
	AbstractItem(const QString &itemName, QTreeWidget *parent = nullptr);
	virtual ~AbstractItem() = 0;
	
	virtual int itemType() const;
	
	inline QString name() const;
	inline void setName(const QString &newName);
};

#include "abstractitem.hpp"
#endif // ABSTRACTITEM_H
