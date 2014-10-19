/***************************************************
** Author: Dmitry Kukovinets (d1021976@gmail.com) **
***************************************************/

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QtWidgets>


#define TASKWIDGET_SIZE_MIN_H			100
#define TASKWIDGET_SIZE_MIN_V			10
#define TASKWIDGET_SIZE_MAX_H			500
#define TASKWIDGET_SIZE_MAX_V			150
#define TASKWIDGET_LABEL_ALIGNMENT		Qt::AlignTop | Qt::AlignLeft
#define TASKWIDGET_FRAME_STYLE			QFrame::Box
#define TASKWIDGET_SIZE_POLICY_H		QSizePolicy::Expanding
#define TASKWIDGET_SIZE_POLICY_V		QSizePolicy::MinimumExpanding


class Task: public QFrame
{
public:
	Task(const QString &name_, QWidget *parent_ = nullptr);
	
	virtual QString label() const;
private:
	QVBoxLayout *mainLayout_;
	QHBoxLayout *progressLayout_;
	QLabel *label_;
	QWidget *progressContainer_;
	QProgressBar *progressBar_;
	QPushButton *startPauseButton_, *cancelButton_;
};

// this (mainLayout)
// 		-> label
// 		-> progressContainer (progressLayout)
// 			-> progressBar
// 			-> startPauseButton
// 			-> cancelButton

#endif	// TASKWIDGET_H
