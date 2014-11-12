// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QtWidgets>

#include "types.h"

class GroupItem: public QTreeWidgetItem
{
	// TODO: Fix 7 errors in moc-file when uncomment these. :|
	
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(id_t id
			   READ id
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
	Q_PROPERTY(QString name
			   READ name
			   WRITE setName
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER true);
public:
	GroupItem(const id_t &id, QTreeWidget *parent = nullptr);
	
	virtual const id_t & id() const;
	
	virtual QString name() const;
	virtual void setName(const QString &new_name);
private:
	id_t id_;
};

#endif // GROUPITEM_H
