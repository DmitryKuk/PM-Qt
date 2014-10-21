// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QtWidgets>

#include "taskbutton.h"

class TaskWidget: public QFrame
{
	Q_OBJECT
	
public:
	// Size
	static const QSize
		SizeMin,
		SizeMax;
	static const QSizePolicy
		SizePolicy;
	
	// Progress
	static const std::pair<int, int>	// (Min, Max)
		ProgressRange;
	
	// Other
	static const Qt::Alignment
		LabelAlignment;
	static const int
		FrameStyle;
	
	
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
// 		-> progressContainer (progressLayout)
// 			-> progressBar
// 			-> actionButton
// 			-> cancelButton

#endif	// TASKWIDGET_H
