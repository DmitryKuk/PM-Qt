// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QtWidgets>

#include "settingssaver.h"

class PasswordContentWidget: public QGroupBox, public SettingsSaver
{
public:
	PasswordContentWidget(const QString &label, QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
private:
	QLabel *label_;
};

#endif // PASSWORDCONTENTWIDGET_H
