// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "warningwindow.h"

WarningWindow::WarningWindow(QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this)),
	okButton_(new QPushButton(tr("OK"), this)),
	mainLayout_(new QVBoxLayout(this)),
	buttonLayout_(new QHBoxLayout(nullptr))
{
	this->okButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	this->buttonLayout_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	this->buttonLayout_->setContentsMargins(0, 0, 0, 0);
	this->buttonLayout_->addWidget(this->okButton_);
	
	this->mainLayout_->setContentsMargins(12, 12, 12, 5);
	this->mainLayout_->addWidget(this->label_);
	this->mainLayout_->addLayout(this->buttonLayout_);
	
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	this->connect(this->okButton_, &QPushButton::clicked,
				  this, &WarningWindow::close);
}


void WarningWindow::setTitle(const QString &title)
{ this->setWindowTitle(title); }
QString WarningWindow::title() const
{ return this->windowTitle(); }


void WarningWindow::setText(const QString &text)
{ this->label_->setText(text); }

QString WarningWindow::text() const
{ return this->label_->text(); }
