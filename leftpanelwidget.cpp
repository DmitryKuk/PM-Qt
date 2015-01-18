// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "leftpanelwidget.h"

LeftPanelWidget::LeftPanelWidget(QWidget *parent):
	QSplitter(Qt::Vertical, parent),
	groupListWidget_(new GroupListWidget(this)),
	taskListWidget_(new TaskListWidget(this))
{
	// Grop list setting...
	this->addWidget(this->groupListWidget_);
	
	// Task list setting...
	this->addWidget(this->taskListWidget_);
	
	// Main setting...
	this->setOrientation(Qt::Vertical);
	this->setCollapsible(0, false);
}


void LeftPanelWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/groupListWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
	this->restoreState(settings.value(current_prefix + "/state").toByteArray());
}

void LeftPanelWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/groupListWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
	settings.setValue(current_prefix + "/state", this->saveState());
}
