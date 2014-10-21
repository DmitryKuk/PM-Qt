// Author: Dmitry Kukovinets (d1021976@gmail.com)
/*
#include "tasklistwidget.h"

// Size
const QSizePolicy
	TaskListWidget::SizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

// Other
const Qt::Alignment
	TaskListWidget::LabelAlignment = Qt::AlignTop | Qt::AlignHCenter;
const int
	TaskListWidget::FrameStyle = QFrame::Box;


TaskListWidget::TaskListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this)),
	listWidget_(new QListWidget(this)),
	mainLayout_(new QVBoxLayout(this))
{
	// UNDER CONSTRUCTION
	// Label setting...
	this->label_->setText(label);
	this->mainLayout_->addWidget(this->label_);
	
	// List widget setting...
	//this->mainLayout_->addWidget(this->listWidget_);
	
	// Main setting...
	this->setLayout(this->mainLayout_);
	this->setFrameStyle(TaskListWidget::FrameStyle);
	this->setSizePolicy(TaskListWidget::SizePolicy);
	new QListViewItem;
}


std::list<TaskWidget *>::iterator TaskListWidget::addTask(const QString &label)
{
	auto *taskWidget = new TaskWidget(label, this);		// Creating of new widget
	this->listWidget_->addWidget(taskWidget);			// Adding it to layout
	return this->insert(this->end(), taskWidget);		// Adding it to std::list
														// ...and returning iterator
}
*/
