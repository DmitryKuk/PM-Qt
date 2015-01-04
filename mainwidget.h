// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>

#include "recordlistwidget.h"
#include "recordcontentwidget.h"
#include "settingssaver.h"

class MainWidget: public QSplitter
{
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(RecordListWidget *recordListWidget
			   READ recordListWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
	Q_PROPERTY(RecordContentWidget *recordContentWidget
			   READ recordContentWidget
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
public:
	MainWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
	
	virtual RecordListWidget * recordListWidget();
	virtual RecordContentWidget * recordContentWidget();
private:
	RecordListWidget *recordListWidget_;
	RecordContentWidget *recordContentWidget_;
};

#endif	// MAINWIDGET_H
