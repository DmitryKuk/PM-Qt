// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef RECORDLISTWIDGET_H
#define RECORDLISTWIDGET_H

#include <QtWidgets>

#include "settingssaver.h"

namespace RecordFieldPos {	// See RecordListWidget constructor!
extern const int
	Name,			// = 0
	TypeName,		// = 1
	ParentGroup;	// = 2
};
	

class RecordListWidget:
	public QTreeWidget,
	public SettingsSaver
{
public:
	RecordListWidget(QWidget *parent = nullptr);
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
};

#endif // RECORDLISTWIDGET_H
