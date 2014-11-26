// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QtWidgets>
#include <QString>

#include "types.h"

class AbstractItem: public QTreeWidgetItem
{
public:
	AbstractItem(id_t id, const QString &itemName, AbstractItem *parent = nullptr);
	AbstractItem(id_t id, const QString &itemName, QWidget *parent = nullptr);
	virtual ~AbstractItem() = 0;
	
	inline id_t id() const;
	
	inline const QString & name() const;
	inline void setName(const QString &newName);
private:
	id_t id_;
};

#endif // ABSTRACTITEM_H
