// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QAction>

#include "settingssaver.h"
#include "leftpanelwidget.h"
#include "mainwidget.h"

class CryptoKernelAgent;

class MainWindow:
	public QMainWindow,
	public SettingsSaver
{
	Q_OBJECT
public:
	MainWindow(CryptoKernelAgent *agent,
			   const QString &title = tr("PM"),
			   bool autoLoadSettings = true,
			   bool autoSaveSettings = true,
			   QWidget *parent = nullptr);
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
	
	inline bool needSaveSettings() const;
	inline void setSaveSettings(bool enable);
	
	inline LeftPanelWidget * leftPanelWidget() const;
	inline GroupListWidget * groupListWidget() const;
	inline TaskListWidget * taskListWidget() const;
	
	inline MainWidget * mainWidget() const;
	inline RecordListWidget * recordListWidget() const;
	inline RecordContentWidget * recordContentWidget() const;
	
	inline CryptoKernelAgent * agent() const;
protected:
	void closeEvent(QCloseEvent *event);
private:
	// Agent
	CryptoKernelAgent *agent_;
	
	
	// Context menu
	// Group list
	QAction
		*addGroupAction_, *addRecordAction_, *addTypeAction_,
		*removeAction_,
		*editTypeAction_;
	
	// Record content
	QAction *addRecordFieldAction_;
	
	QSplitter *mainSplit_;
	LeftPanelWidget *leftPanelWidget_;	// Left panel
	MainWidget *mainWidget_;			// Main widget
	
	
	// Other
	bool saveSettings_;
};

#include "mainwindow.hpp"
#endif	// MAINWINDOW_H
