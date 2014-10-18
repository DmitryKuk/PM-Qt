/***************************************************
** Author: Dmitry Kukovinets (d1021976@gmail.com)
****************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>

class MainWindow:
	public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
};

#endif	// MAINWINDOW_H
