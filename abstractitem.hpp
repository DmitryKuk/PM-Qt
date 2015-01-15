// Author: Dmitry Kukovinets (d1021976@gmail.com)

inline int itemType(const QTreeWidgetItem *item)
{ return item->data(0, Qt::UserRole).toInt(); }

inline void setItemType(QTreeWidgetItem *item, int type)
{ item->setData(0, Qt::UserRole, type); }
