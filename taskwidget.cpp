/***************************************************
** Author: Dmitry Kukovinets (d1021976@gmail.com) **
***************************************************/

#include "taskwidget.h"

Task::Task(const QString &name_, QWidget *parent_):
	QFrame(parent_),
	mainLayout_(new QVBoxLayout(this)),
	progressLayout_(new QHBoxLayout(this)),
	label_(new QLabel(this)),
	progressContainer_(new QWidget(this)),
	progressBar_(new QProgressBar(progressContainer_)),
	startPauseButton_(new QPushButton("Start/Pause", progressContainer_)),
	cancelButton_(new QPushButton("Cancel", progressContainer_))
{
	// Label setting...
	this->label_->setText(name_);
	this->label_->setAlignment(TASKWIDGET_LABEL_ALIGNMENT);
	this->mainLayout_->addWidget(this->label_);
	
	// Progress setting...
	this->progressLayout_->addWidget(this->progressBar_);
	this->progressLayout_->addWidget(this->startPauseButton_);
	this->progressLayout_->addWidget(this->cancelButton_);
	this->progressContainer_->setLayout(this->progressLayout_);
	this->mainLayout_->addWidget(this->progressContainer_);
	
	// Main setting...
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(TASKWIDGET_FRAME_STYLE);
	this->setMinimumSize(TASKWIDGET_SIZE_MIN_H, TASKWIDGET_SIZE_MIN_V);
	this->setMaximumSize(TASKWIDGET_SIZE_MAX_H, TASKWIDGET_SIZE_MAX_V);
	this->setSizePolicy(TASKWIDGET_SIZE_POLICY_H, TASKWIDGET_SIZE_POLICY_V);
}

QString Task::label() const
{
	return this->label_->text();
}
