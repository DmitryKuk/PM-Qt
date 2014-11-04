// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordcontentwidget.h"

PasswordContentWidget::PasswordContentWidget(const QString &label, QWidget *parent):
	QGroupBox(parent),
	label_(new QLabel(this))
{
	this->label_->setText(label);
}


void PasswordContentWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/passwordContentWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void PasswordContentWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/passwordContentWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}
