// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPLISTWIDGET_H
#define GROUPLISTWIDGET_H

#include <QTreeWidget>

#include "settingssaver.h"

class GroupListWidget:
	public QTreeWidget,
	public SettingsSaver
{
	Q_OBJECT
public:
	GroupListWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
};

#endif	// GROUPLISTWIDGET_H
