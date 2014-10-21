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
	// Size
	static const QSizePolicy
		SizePolicy;
	
	// Other
	static const Qt::Alignment
		LabelAlignment;
	static const int
		FrameStyle;
	
	
	TaskListWidget(const QString &label, QWidget *parent = nullptr);
	
	//virtual std::list<TaskWidget *>::iterator addTask(const QString &label);
private:
	QLabel *label_;
	QListWidget *listWidget_;
	QVBoxLayout *mainLayout_;
};

#endif	// TASKLISTWIDGET_H
