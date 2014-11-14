// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDITEM_H
#define PASSWORDITEM_H

#include <QtWidgets>
#include <QStringList>

#include "types.h"

class PasswordItem: public QTreeWidgetItem
{
public:
	PasswordItem(const id_t &id, const QStringList &passwordProperties, QTreeWidget *parent = nullptr);
	
	virtual const id_t & id() const;
private:
	id_t id_;
};

#endif // PASSWORDITEM_H
