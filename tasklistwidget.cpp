// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "tasklistwidget.h"

TaskListWidget::TaskListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this)),
	scrollArea_(new QScrollArea(this)),
	mainLayout_(new QVBoxLayout(this)),
	listLayout_(new QVBoxLayout(scrollArea_))
{
	// Label setting...
	this->label_->setText(label);
	this->label_->setAlignment(Qt::AlignHCenter);
	this->label_->setSizePolicy(QSizePolicy::Expanding,
								QSizePolicy::Fixed);
	this->mainLayout_->addWidget(this->label_);
	
	// List setting...
	this->listLayout_->setContentsMargins(0, 0, 0, 0);	// Left, Top, Right, Bottom
	this->scrollArea_->setLayout(listLayout_);
	this->scrollArea_->setContentsMargins(0, 0, 0, 0);	// Left, Top, Right, Bottom
	this->mainLayout_->addWidget(this->scrollArea_);
	
	// Main setting...
	this->mainLayout_->setContentsMargins(0, 0, 0, 0);	// Left, Top, Right, Bottom
	this->setMinimumSize(this->label_->minimumSize());
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(QFrame::Box);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
}


TaskListWidget::Container::const_iterator TaskListWidget::addTask(const QString &label)
{
	auto *taskWidget = new TaskWidget(label, this);
	this->listLayout_->addWidget(taskWidget);
	return TaskListWidget::Container::insert(TaskListWidget::Container::end(),
											 taskWidget);
}


TaskListWidget::Container::const_iterator TaskListWidget::begin() const
{
	return TaskListWidget::Container::begin();
}


TaskListWidget::Container::const_iterator TaskListWidget::end() const
{
	return TaskListWidget::Container::end();
}
