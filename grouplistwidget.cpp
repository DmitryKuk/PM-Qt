// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "grouplistwidget.h"

// TODO: Remove this test (some groups)
#include "groupitem.h"

GroupListWidget::GroupListWidget(QWidget *parent):
	QTreeWidget(parent)
{
	this->setColumnCount(1);
	this->setHeaderLabel(tr("Groups"));
	
	GroupItem *p = new GroupItem(1, "Home", this);
	p = new GroupItem(2, "Copmuter passwords", p);
	new GroupItem(3, "Linux", p);
	new GroupItem(4, "Windows", p);
	new GroupItem(5, "Etc", p);
	
	p = new GroupItem(6, "Work", this);
	new GroupItem(7, "E-mail", p);
}


void GroupListWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/groupListWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void GroupListWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/groupListWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}
