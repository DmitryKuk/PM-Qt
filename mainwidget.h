// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>

#include "passwordlistwidget.h"
#include "passwordcontentwidget.h"
#include "settingssaver.h"

class MainWidget: public QSplitter
{
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(PasswordListWidget *passwordListWidget
			   READ passwordListWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
	Q_PROPERTY(PasswordContentWidget *passwordContentWidget
			   READ passwordContentWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
public:
	MainWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
	
	virtual PasswordListWidget * passwordListWidget();
	virtual PasswordContentWidget * passwordContentWidget();
private:
	PasswordListWidget *passwordListWidget_;
	PasswordContentWidget *passwordContentWidget_;
};

#endif	// MAINWIDGET_H
