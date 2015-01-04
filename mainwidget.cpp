// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent):
	QSplitter(parent),
	recordListWidget_(new RecordListWidget(this)),
	recordContentWidget_(new RecordContentWidget(this))
{
	// Record list widget setting...
	this->addWidget(this->recordListWidget_);
	
	// Record content widget setting...
	this->addWidget(this->recordContentWidget_);
	
	// Main setting...
	this->setOrientation(Qt::Vertical);
}


void MainWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/mainWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
	this->restoreState(settings.value(current_prefix + "/state").toByteArray());
	
	// Child widgets
	this->recordListWidget_->readSettings(settings, current_prefix);
	this->recordContentWidget_->readSettings(settings, current_prefix);
}

void MainWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/mainWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
	settings.setValue(current_prefix + "/state", this->saveState());
	
	// Child widgets
	this->recordListWidget_->writeSettings(settings, current_prefix);
	this->recordContentWidget_->writeSettings(settings, current_prefix);
}


RecordListWidget * MainWidget::recordListWidget()
{
	return this->recordListWidget_;
}


RecordContentWidget * MainWidget::recordContentWidget()
{
	return this->recordContentWidget_;
}
