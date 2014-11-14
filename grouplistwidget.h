// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPLISTWIDGET_H
#define GROUPLISTWIDGET_H

#include <QtWidgets>

#include "settingssaver.h"

class GroupListWidget:
	public QTreeWidget,
	public SettingsSaver
{
	Q_OBJECT
public:
	GroupListWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
};

#endif	// GROUPLISTWIDGET_H
