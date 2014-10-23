// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef GROUPLISTWIDGET_H
#define GROUPLISTWIDGET_H

#include <QtWidgets>

class GroupListWidget: public QFrame
{
public:
	GroupListWidget(const QString &label, QWidget *parent = nullptr);
private:
	QLabel *label_;
};

#endif	// GROUPLISTWIDGET_H
