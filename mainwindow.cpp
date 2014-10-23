// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwindow.h"

MainWindow::MainWindow(const QString &title, QWidget *parent):
	QMainWindow(parent),
	mainSplit_(new QSplitter(this)),
	leftPanelWidget_(new LeftPanelWidget(mainSplit_)),
	mainWidget_(new MainWidget(mainSplit_))
{
	// TaskWidgets
	// auto *widget = new QWidget(this);
	// QVBoxLayout *layout = new QVBoxLayout(widget);
	// widget->setLayout(layout);
	
	// auto *widget1 = new TaskWidget("Some task 1", widget);
	// auto *widget2 = new TaskWidget("Some task 2", widget);
	// auto *widget3 = new TaskWidget("Some task 3", widget);
	
	// layout->addWidget(widget1);
	// layout->addWidget(widget2);
	// layout->addWidget(widget3);
	
	// widget1->setProgress(50);
	// widget2->setProgress(30);
	// widget3->setProgress(75);
	
	
	// TaskListWidget
	// auto *widget = new TaskListWidget("Tasks", this);
	// widget->addTask("First task");
	// widget->addTask("Second task");
	
	
	// Left panel widget
	// auto *widget = new LeftPanelWidget(this);
	// auto *taskListWidget = widget->taskListWidget();
	
	// auto it1 = taskListWidget->addTask("Some task 1");
	// auto it2 = taskListWidget->addTask("Some task 2");
	// auto it3 = taskListWidget->addTask("Some task 3");
	
	// (*it1)->setProgress(50);
	// (*it2)->setProgress(30);
	// (*it3)->setProgress(75);
	
	
	// Main splitter setting...
	this->mainSplit_->setOrientation(Qt::Horizontal);
	this->mainSplit_->addWidget(this->leftPanelWidget_);
	this->mainSplit_->addWidget(this->mainWidget_);
	
	// Main setting...
	this->setWindowTitle(title);
	this->setCentralWidget(this->mainSplit_);
}


LeftPanelWidget * MainWindow::leftPanelWidget()
{
	return this->leftPanelWidget_;
}


MainWidget * MainWindow::mainWidget()
{
	return this->mainWidget_;
}
