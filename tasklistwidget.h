// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H

#include <list>

#include <QtWidgets>

#include "taskwidget.h"

class TaskListWidget:
	public QFrame,
	private std::list<TaskWidget *>
{
	Q_OBJECT
	
public:
	typedef std::list<TaskWidget *> Container;
	
	TaskListWidget(const QString &label, QWidget *parent = nullptr);
	
	virtual Container::const_iterator addTask(const QString &label);
	
	Container::const_iterator begin() const;
	Container::const_iterator end() const;
private:
	QLabel *label_;
	QScrollArea *scrollArea_;
	QVBoxLayout *mainLayout_, *listLayout_;
};

#endif	// TASKLISTWIDGET_H
