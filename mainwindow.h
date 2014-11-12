// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>
#include <QtCore>

#include "settingssaver.h"
#include "leftpanelwidget.h"
#include "mainwidget.h"

class MainWindow:
	public QMainWindow,
	public SettingsSaver
{
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(bool saveSettings
			   READ needSaveSettings
			   WRITE setSaveSettings
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER true);
public:
	MainWindow(const QString &title = tr("PM"),
			   bool autoLoadSettings = true,
			   bool autoSaveSettings = true,
			   QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "");
	void writeSettings(QSettings &settings, const QString &prefix = "") const;
	
	bool needSaveSettings() const;
	void setSaveSettings(bool enable);
	
	virtual LeftPanelWidget * leftPanelWidget();
	virtual MainWidget * mainWidget();
protected:
	void closeEvent(QCloseEvent *event);
private:
	QSplitter *mainSplit_;
	LeftPanelWidget *leftPanelWidget_;
	MainWidget *mainWidget_;
	
	bool saveSettings_;
};

#endif	// MAINWINDOW_H
