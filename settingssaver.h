// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include <QSettings>

// Simple non-Qt class that allows to write and read
// current widget settings.
class SettingsSaver
{
public:
	virtual ~SettingsSaver() = 0;
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") = 0;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const = 0;
};

#endif // SETTINGSSAVER_H
