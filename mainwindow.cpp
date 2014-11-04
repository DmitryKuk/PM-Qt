// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwindow.h"

MainWindow::MainWindow(const QString &title,
					   bool autoLoadSettings,
					   bool autoSaveSettings,
					   QWidget *parent):
	QMainWindow(parent),
	mainSplit_(new QSplitter(this)),
	leftPanelWidget_(new LeftPanelWidget(mainSplit_)),
	mainWidget_(new MainWidget(mainSplit_)),
	saveSettings_(autoSaveSettings? true: false)
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
	
	if (autoLoadSettings) {
		QSettings settings;
		this->readSettings(settings);
	}
	
	// Main splitter setting...
	this->mainSplit_->setOrientation(Qt::Horizontal);
	this->mainSplit_->addWidget(this->leftPanelWidget_);
	this->mainSplit_->addWidget(this->mainWidget_);
	
	// Main setting...
	this->setWindowTitle(title);
	this->setCentralWidget(this->mainSplit_);
}


void MainWindow::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/mainWindow";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
	this->restoreState(settings.value(current_prefix + "/state").toByteArray());
	
	// Child widgets
	this->mainSplit_->restoreGeometry(settings.value(current_prefix + "/mainSplit/geometry").toByteArray());
	this->mainSplit_->restoreState(settings.value(current_prefix + "/mainSplit/state").toByteArray());
	
	this->leftPanelWidget_->readSettings(settings, current_prefix);
	this->mainWidget_->readSettings(settings, current_prefix);
}

void MainWindow::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/mainWindow";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
	settings.setValue(current_prefix + "/state", this->saveState());
	
	// Child widgets
	settings.setValue(current_prefix + "/mainSplit/geometry", this->mainSplit_->saveGeometry());
	settings.setValue(current_prefix + "/mainSplit/state", this->mainSplit_->saveState());
	
	this->leftPanelWidget_->writeSettings(settings, current_prefix);
	this->mainWidget_->writeSettings(settings, current_prefix);
}


bool MainWindow::needSaveSettings() const
{
	return this->saveSettings_;
}

void MainWindow::setSaveSettings(bool enable)
{
	this->saveSettings_ = enable;
}


LeftPanelWidget * MainWindow::leftPanelWidget()
{
	return this->leftPanelWidget_;
}


MainWidget * MainWindow::mainWidget()
{
	return this->mainWidget_;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	if (this->needSaveSettings()) {
		QSettings settings;
		this->writeSettings(settings);
	}
	this->QMainWindow::closeEvent(event);
}
