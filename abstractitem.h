// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QString>

// Constants for identification of selected elements in groupListWidget
// Set one of these with setData(0, Qt::UserRole, oneOfThese)
namespace ItemType {
extern const int
	Abstract,		// =  5
	Record,			// = 10
	Group,			// = 20
	Type,			// = 30
	RootGroup,		// = 40
	RootTypeGroup;	// = 50
};

extern inline int itemType(const QTreeWidgetItem *item);
extern inline void setItemType(QTreeWidgetItem *item, int type);


class AbstractItem: public QTreeWidgetItem
{
public:
	AbstractItem(const QString &itemName, AbstractItem *parent);
	AbstractItem(const QString &itemName, QTreeWidget *parent = nullptr);
	virtual ~AbstractItem() = 0;
	
	QString name() const;
	void setName(const QString &newName);
};

#include "abstractitem.hpp"
#endif // ABSTRACTITEM_H
