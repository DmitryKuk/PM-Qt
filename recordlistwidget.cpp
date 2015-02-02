// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordlistwidget.h"

// See RecordListWidget constructor!
extern const int
	RecordFieldPos::Name		= 0,
	RecordFieldPos::TypeName	= 1,
	RecordFieldPos::ParentGroup = 2;


RecordListWidget::RecordListWidget(QWidget *parent):
	QTreeWidget(parent)
{
	this->setHeaderLabels({ tr("Name"), tr("Type"), tr("Group") });
	this->setAlternatingRowColors(true);
}


void RecordListWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/recordListWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void RecordListWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/recordListWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}
