// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H

#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>

#include <list>

#include "taskwidget.h"
#include "settingssaver.h"

class TaskListWidget:
	public QFrame,
	private std::list<TaskWidget *>,
	public SettingsSaver
{
	Q_OBJECT
public:
	typedef std::list<TaskWidget *> Container;
	
	using Container::const_iterator;
	using Container::begin;
	using Container::end;
	
	
	TaskListWidget(QWidget *parent = nullptr);
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
	
	Container::const_iterator addTask(const QString &label);
private:
	using Container::iterator;
	
	QLabel *label_;
	QScrollArea *scrollArea_;
	QVBoxLayout *mainLayout_, *listLayout_;
};

#endif	// TASKLISTWIDGET_H
