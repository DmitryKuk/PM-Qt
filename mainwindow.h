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

class CryptoKernelAgent;

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
	MainWindow(CryptoKernelAgent *agent,
			   const QString &title = tr("PM"),
			   bool autoLoadSettings = true,
			   bool autoSaveSettings = true,
			   QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
	
	bool needSaveSettings() const;
	void setSaveSettings(bool enable);
	
	LeftPanelWidget * leftPanelWidget() const;
	MainWidget * mainWidget() const;
	
	CryptoKernelAgent * agent() const;
	
	void updateRecordListItems();
protected:
	void closeEvent(QCloseEvent *event);
private:
	CryptoKernelAgent *agent_;
	
	QSplitter *mainSplit_;
	LeftPanelWidget *leftPanelWidget_;
	MainWidget *mainWidget_;
	
	bool saveSettings_;
};

#endif	// MAINWINDOW_H
