// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LEFTPANELWIDGET_H
#define LEFTPANELWIDGET_H

#include <QtWidgets>

#include "grouplistwidget.h"
#include "tasklistwidget.h"
#include "settingssaver.h"

class LeftPanelWidget:
	public QSplitter,
	public SettingsSaver
{
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(GroupListWidget *groupListWidget
			   READ groupListWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
	Q_PROPERTY(TaskListWidget *taskListWidget
			   READ taskListWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
public:
	LeftPanelWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
	
	GroupListWidget * groupListWidget();
	TaskListWidget * taskListWidget();
private:
	GroupListWidget *groupListWidget_;
	TaskListWidget *taskListWidget_;
};

#endif	// LEFTPANELWIDGET_H
