// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QFrame>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QString>

#include "taskbutton.h"

class TaskWidget: public QFrame
{
	Q_OBJECT
public:
	TaskWidget(const QString &label, QWidget *parent = nullptr);
	
	virtual QString label() const;
	
	int progress() const;
	void setProgress(int value);
private:
	QVBoxLayout *mainLayout_;
	QHBoxLayout *progressLayout_;
	QLabel *label_;
	QProgressBar *progressBar_;
	TaskButton *actionButton_, *cancelButton_;
};

#endif	// TASKWIDGET_H
