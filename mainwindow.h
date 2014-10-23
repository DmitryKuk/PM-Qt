// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>

#include "leftpanelwidget.h"
#include "mainwidget.h"

class MainWindow: public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(const QString &title = tr("PM"), QWidget *parent = nullptr);
	
	virtual LeftPanelWidget * leftPanelWidget();
	virtual MainWidget * mainWidget();
private:
	QSplitter *mainSplit_;
	LeftPanelWidget *leftPanelWidget_;
	MainWidget *mainWidget_;
};

#endif	// MAINWINDOW_H
