// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LEFTPANELWIDGET_H
#define LEFTPANELWIDGET_H

#include <QtWidgets>

#include "grouplistwidget.h"
#include "tasklistwidget.h"

class LeftPanelWidget: public QSplitter
{
public:
	LeftPanelWidget(QWidget *parent = nullptr);
	
	GroupListWidget * groupListWidget();
	TaskListWidget * taskListWidget();
};

#endif	// LEFTPANELWIDGET_H
