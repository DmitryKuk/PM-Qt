// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QtWidgets>

#include "settingssaver.h"

class RecordContentWidget:
	public QGroupBox,
	public SettingsSaver
{
	Q_OBJECT
public:
	RecordContentWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
private:
	QLabel *label_;
};

#endif // PASSWORDCONTENTWIDGET_H
