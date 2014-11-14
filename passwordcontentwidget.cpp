// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordcontentwidget.h"

PasswordContentWidget::PasswordContentWidget(QWidget *parent):
	QGroupBox(parent),
	label_(new QLabel(tr("Password content"), this))
{}


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
