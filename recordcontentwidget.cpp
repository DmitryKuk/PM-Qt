// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordcontentwidget.h"

RecordContentWidget::RecordContentWidget(QWidget *parent):
	QGroupBox(parent),
	label_(new QLabel(tr("Password content"), this))
{}


void RecordContentWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/recordContentWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void RecordContentWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/recordContentWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}
