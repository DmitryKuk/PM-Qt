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


PasswordListWidget * MainWidget::passwordListWidget()
{
	return this->passwordListWidget_;
}


PasswordContentWidget * MainWidget::passwordContentWidget()
{
	return this->passwordContentWidget_;
}
