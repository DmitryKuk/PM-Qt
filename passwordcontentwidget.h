// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QtWidgets>

class PasswordContentWidget: public QGroupBox
{
public:
	PasswordContentWidget(const QString &label, QWidget *parent = nullptr);
private:
	QLabel *label_;
};

#endif // PASSWORDCONTENTWIDGET_H
