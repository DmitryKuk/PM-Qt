// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "tasklistwidget.h"

// Size
const QSizePolicy
	TaskListWidget::SizePolicy(QSizePolicy::MinimumExpanding,
							   QSizePolicy::MinimumExpanding);

// Other
const Qt::Alignment
	TaskListWidget::LabelAlignment = Qt::AlignTop | Qt::AlignHCenter;
const int
	TaskListWidget::FrameStyle = QFrame::Box;


TaskListWidget::TaskListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this)),
	mainLayout_(new QVBoxLayout(this)),
	listLayout_(new QVBoxLayout(this))
{
	// Label setting...
	this->label_->setText(label);
	this->mainLayout_->addWidget(this->label_);
	
	// List layout setting...
	this->mainLayout_->addLayout(this->listLayout_);
	
	// Main setting...
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(TaskListWidget::FrameStyle);
	this->setSizePolicy(TaskListWidget::SizePolicy);
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
