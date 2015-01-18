// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LEFTPANELWIDGET_H
#define LEFTPANELWIDGET_H

#include <QSplitter>

#include "grouplistwidget.h"
#include "tasklistwidget.h"
#include "settingssaver.h"

class LeftPanelWidget:
	public QSplitter,
	public SettingsSaver
{
	Q_OBJECT
public:
	LeftPanelWidget(QWidget *parent = nullptr);
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
	
	inline GroupListWidget * groupListWidget() const;
	inline TaskListWidget * taskListWidget() const;
private:
	GroupListWidget *groupListWidget_;
	TaskListWidget *taskListWidget_;
};

#include "leftpanelwidget.hpp"
#endif	// LEFTPANELWIDGET_H
