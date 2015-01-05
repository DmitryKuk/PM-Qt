// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QString>

class AbstractItem: public QTreeWidgetItem
{
public:
	AbstractItem(const QString &itemName, AbstractItem *parent = nullptr);
	AbstractItem(const QString &itemName, QTreeWidget *parent = nullptr);
	virtual ~AbstractItem() = 0;
	
	QString name() const;
	void setName(const QString &newName);
};

#endif // ABSTRACTITEM_H
