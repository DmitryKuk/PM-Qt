// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordlistwidget.h"

PasswordListWidget::PasswordListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this))
{
	this->label_->setText(label);
}


void PasswordListWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/passwordListWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void PasswordListWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/passwordListWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}
