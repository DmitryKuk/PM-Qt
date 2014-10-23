// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "leftpanelwidget.h"

LeftPanelWidget::LeftPanelWidget(QWidget *parent):
	QSplitter(Qt::Vertical, parent)
{
	this->addWidget(new GroupListWidget(tr("Groups"), this));
	this->addWidget(new TaskListWidget(tr("Tasks"), this));
}


GroupListWidget * LeftPanelWidget::groupListWidget()
{
	return dynamic_cast<GroupListWidget *>(this->widget(0));
}


TaskListWidget * LeftPanelWidget::taskListWidget()
{
	return dynamic_cast<TaskListWidget *>(this->widget(1));
}
