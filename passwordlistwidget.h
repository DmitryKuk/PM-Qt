// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDLISTWIDGET_H
#define PASSWORDLISTWIDGET_H

#include <QtWidgets>

class PasswordListWidget: public QFrame
{
public:
	PasswordListWidget(const QString &label, QWidget *parent = nullptr);
private:
	QLabel *label_;
};

#endif // PASSWORDLISTWIDGET_H
