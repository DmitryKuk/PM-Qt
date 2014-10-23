// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "leftpanelwidget.h"

LeftPanelWidget::LeftPanelWidget(QWidget *parent):
	QSplitter(Qt::Vertical, parent),
	groupListWidget_(new GroupListWidget(tr("Groups"), this)),
	taskListWidget_(new TaskListWidget(tr("Tasks"), this))
{
	// Grop list setting...
	this->addWidget(this->groupListWidget_);
	
	// Task list setting...
	this->addWidget(this->taskListWidget_);
	
	// Main setting...
	this->setOrientation(Qt::Vertical);
}


GroupListWidget * LeftPanelWidget::groupListWidget()
{
	return this->groupListWidget_;
}


TaskListWidget * LeftPanelWidget::taskListWidget()
{
	return this->taskListWidget_;
}
