// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "taskwidget.h"

// Size
const QSize
	TaskWidget::SizeMin(100, 70),
	TaskWidget::SizeMax(500, 150);

const QSizePolicy
	TaskWidget::SizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

// Progress
const std::pair<int, int>
	TaskWidget::ProgressRange(0, 100);

// Other
const Qt::Alignment
	TaskWidget::LabelAlignment = Qt::AlignTop | Qt::AlignLeft;
const int
	TaskWidget::FrameStyle = QFrame::Box;


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
	this->label_->setAlignment(TaskWidget::LabelAlignment);
	this->mainLayout_->addWidget(this->label_);
	
	// Progress setting...
	this->progressBar_->setRange(TaskWidget::ProgressRange.first,
								 TaskWidget::ProgressRange.second);
	this->progressBar_->setValue(TaskWidget::ProgressRange.first);
	this->progressLayout_->addWidget(this->progressBar_);
	this->progressLayout_->addWidget(this->actionButton_);
	this->progressLayout_->addWidget(this->cancelButton_);
	this->mainLayout_->addLayout(this->progressLayout_);
	
	// Main setting...
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(TaskWidget::FrameStyle);
	this->setMinimumSize(TaskWidget::SizeMin);
	this->setMaximumSize(TaskWidget::SizeMax);
	this->setSizePolicy(TaskWidget::SizePolicy);
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
	if (value >= TaskWidget::ProgressRange.first
		&& value <= TaskWidget::ProgressRange.second)
		this->progressBar_->setValue(value);
}
