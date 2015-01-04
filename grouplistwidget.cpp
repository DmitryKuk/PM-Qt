// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "grouplistwidget.h"

GroupListWidget::GroupListWidget(QWidget *parent):
	QTreeWidget(parent)
{
	this->setColumnCount(1);
	this->setHeaderLabel(tr("Filters"));
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
