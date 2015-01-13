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
	
	// Update functions
	void updateRecordListItems();
	void updateRecordContent();
protected:
	void closeEvent(QCloseEvent *event);
private:
	// Slots
	// Record content
	void onNameClicked();
	void onNameChanged(QString newName);
	
	void onTypeClicked();
	void onTypeChanged(QString newTypeName);
	
	void onGroupClicked();
	void onGroupChanged(QString newGroupName);
	
	void onFieldClicked(int index);
	void onFieldChanged(int index, QString newText);
	
	// Adding new groups/records/types
	void onAddGroupActionActivated();
	void onAddRecordActionActivated();
	void onAddTypeActionActivated();
	void onRemoveActionActivated();
	
	
	// Agent
	CryptoKernelAgent *agent_;
	
	
	// Context menu
	QAction *addGroupAction_, *addRecordAction_, *addTypeAction_, *removeAction_;
	
	QSplitter *mainSplit_;
	LeftPanelWidget *leftPanelWidget_;	// Left panel
	MainWidget *mainWidget_;			// Main widget
	
	// Other
	bool saveSettings_;
};

#endif	// MAINWINDOW_H
