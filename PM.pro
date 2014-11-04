#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T19:16:09
#
#-------------------------------------------------

QT += core gui
QMAKE_MAC_SDK = macosx10.9
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../PM
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp\
		taskwidget.cpp\
		tasklistwidget.cpp\
		leftpanelwidget.cpp\
		mainwidget.cpp\
		grouplistwidget.cpp\
		passwordlistwidget.cpp\
		passwordcontentwidget.cpp\
		settingssaver.cpp

HEADERS +=\
		mainwindow.h\
		taskwidget.h\
		tasklistwidget.h\
		leftpanelwidget.h\
		mainwidget.h\
		grouplistwidget.h\
		passwordlistwidget.h\
		passwordcontentwidget.h\
		settingssaver.h
