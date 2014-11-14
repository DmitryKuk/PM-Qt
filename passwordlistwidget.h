// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDLISTWIDGET_H
#define PASSWORDLISTWIDGET_H

#include <QtWidgets>

#include "settingssaver.h"

class PasswordListWidget:
	public QTreeWidget,
	public SettingsSaver
{
public:
	PasswordListWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
};

#endif // PASSWORDLISTWIDGET_H
