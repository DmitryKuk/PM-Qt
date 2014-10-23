// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>

#include "passwordlistwidget.h"
#include "passwordcontentwidget.h"

class MainWidget: public QSplitter
{
	Q_OBJECT
	
public:
	MainWidget(QWidget *parent = nullptr);
	
	virtual PasswordListWidget * passwordListWidget();
	virtual PasswordContentWidget * passwordContentWidget();
private:
	PasswordListWidget *passwordListWidget_;
	PasswordContentWidget *passwordContentWidget_;
};

#endif	// MAINWIDGET_H
