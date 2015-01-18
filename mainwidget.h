// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QSplitter>

#include "recordlistwidget.h"
#include "recordcontentwidget.h"
#include "settingssaver.h"

class MainWidget:
	public QSplitter,
	public SettingsSaver
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = nullptr);
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
	
	inline RecordListWidget * recordListWidget() const;
	inline RecordContentWidget * recordContentWidget() const;
private:
	RecordListWidget *recordListWidget_;
	RecordContentWidget *recordContentWidget_;
};

#include "mainwidget.hpp"
#endif	// MAINWIDGET_H
