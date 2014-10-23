// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "taskwidget.h"

TaskWidget::TaskWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	mainLayout_(new QVBoxLayout(this)),
	progressLayout_(new QHBoxLayout(this)),
	label_(new QLabel(this)),
	progressBar_(new QProgressBar(this)),
	actionButton_(new TaskButton("Action", this)),
	cancelButton_(new TaskButton("Cancel", this))
{
	// Label setting...
	this->label_->setText(label);
	this->label_->setAlignment(Qt::AlignLeft);
	this->label_->setSizePolicy(QSizePolicy::Expanding,
								QSizePolicy::Fixed);
	this->mainLayout_->addWidget(this->label_);
	
	// Progress setting...
	this->progressBar_->setSizePolicy(QSizePolicy::Expanding,
									  QSizePolicy::Fixed);
	this->progressLayout_->addWidget(this->progressBar_);
	this->progressLayout_->addWidget(this->actionButton_);
	this->progressLayout_->addWidget(this->cancelButton_);
	this->mainLayout_->addLayout(this->progressLayout_);
	
	// Main setting...
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(QFrame::Box);
	
	// Minimum sizes of widgets
	QSize
		labelMinSize = this->label_->minimumSize(),
		progressBarMinSize = this->progressBar_->minimumSize(),
		actionButtonMinSize = this->actionButton_->minimumSize(),
		cancelButtonMinSize = this->cancelButton_->minimumSize();
	
	// Total minimum size
	QSize minSize(
			std::max(labelMinSize.width(),
					 progressBarMinSize.width()
					 + actionButtonMinSize.width()
					 + cancelButtonMinSize.width()),
			labelMinSize.height()
			+ std::max(progressBarMinSize.height(),
					   std::max(actionButtonMinSize.height(),
					   			cancelButtonMinSize.height())));
	
	this->setMinimumSize(minSize);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}


QString TaskWidget::label() const
{
	return this->label_->text();
}


int TaskWidget::progress() const
{
	return this->progressBar_->value();
}


void TaskWidget::setProgress(int value)
{
	if (value >= 0 && value <= 100)
		this->progressBar_->setValue(value);
}
