// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>

#include "leftpanelwidget.h"
#include "centralwidget.h"

class MainWindow:
	public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(const QString &title = tr("PM"), QWidget *parent = nullptr);
	~MainWindow();
};

#endif	// MAINWINDOW_H
