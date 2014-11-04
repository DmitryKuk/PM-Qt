// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent):
	QSplitter(parent),
	passwordListWidget_(new PasswordListWidget(tr("Passwords"), this)),
	passwordContentWidget_(new PasswordContentWidget(tr("Password content"), this))
{
	// Password list widget setting...
	this->addWidget(this->passwordListWidget_);
	
	// Password content widget setting...
	this->addWidget(this->passwordContentWidget_);
	
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
	this->passwordListWidget_->readSettings(settings, current_prefix);
	this->passwordContentWidget_->readSettings(settings, current_prefix);
}

void MainWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/mainWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
	settings.setValue(current_prefix + "/state", this->saveState());
	
	// Child widgets
	this->passwordListWidget_->writeSettings(settings, current_prefix);
	this->passwordContentWidget_->writeSettings(settings, current_prefix);
}


PasswordListWidget * MainWidget::passwordListWidget()
{
	return this->passwordListWidget_;
}


PasswordContentWidget * MainWidget::passwordContentWidget()
{
	return this->passwordContentWidget_;
}
