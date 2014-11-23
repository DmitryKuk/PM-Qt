// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordlistwidget.h"

#include <initializer_list>

// TODO: Remove this test (some groups)
#include "passworditem.h"

PasswordListWidget::PasswordListWidget(QWidget *parent):
	QTreeWidget(parent)
{
	this->setHeaderLabels({ tr("Name"), tr("Group"), tr("Type"), tr("Modified"), tr("Created") });
	
	new PasswordItem(3, { "Linux"  , "Example passwords", "Password", "02.01.2100", "01.01.2100" }, this);
	new PasswordItem(4, { "Windows", "Example passwords", "Password", "02.01.2100", "01.01.2100" }, this);
	new PasswordItem(5, { "Etc"    , "Example passwords", "Password", "02.01.2100", "01.01.2100" }, this);
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
