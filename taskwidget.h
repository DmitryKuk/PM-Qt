// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QtWidgets>

#include "taskbutton.h"

class TaskWidget: public QFrame
{
	Q_OBJECT
	
	// Properties
	Q_PROPERTY(int progress
			   READ progress
			   WRITE setProgress
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
	Q_PROPERTY(QString label
			   READ label
			   DESIGNABLE false
			   SCRIPTABLE false
			   USER false);
public:
	TaskWidget(const QString &label, QWidget *parent = nullptr);
	
	virtual QString label() const;
	
	virtual int progress() const;
	virtual void setProgress(int value);
private:
	QVBoxLayout *mainLayout_;
	QHBoxLayout *progressLayout_;
	QLabel *label_;
	QProgressBar *progressBar_;
	TaskButton *actionButton_, *cancelButton_;
};

// this (mainLayout)
// 		-> label
// 		-> (progressLayout)
// 			-> progressBar
// 			-> actionButton
// 			-> cancelButton

#endif	// TASKWIDGET_H
