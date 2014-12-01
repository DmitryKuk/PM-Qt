#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T19:16:09
#
#-------------------------------------------------

QT += core gui
QMAKE_MAC_SDK = macosx10.9
QMAKE_CXXFLAGS += -std=c++11 -Wall
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PM
TEMPLATE = app


SOURCES += main.cpp\
		cryptokernel/types.cpp\
		mainwindow.cpp\
		taskwidget.cpp\
		tasklistwidget.cpp\
		leftpanelwidget.cpp\
		mainwidget.cpp\
		grouplistwidget.cpp\
		passwordlistwidget.cpp\
		passwordcontentwidget.cpp\
		settingssaver.cpp\
		abstractitem.cpp\
		groupitem.cpp\
		passworditem.cpp\
		cryptokernelagent.cpp\
		cryptokernel/cryptokernel.cpp

HEADERS += cryptokernel/types.h types.h\
		mainwindow.h\
		taskwidget.h\
		tasklistwidget.h\
		leftpanelwidget.h\
		mainwidget.h\
		grouplistwidget.h\
		passwordlistwidget.h\
		passwordcontentwidget.h\
		settingssaver.h\
		abstractitem.h\
		groupitem.h\
		passworditem.h\
		cryptokernelagent.h\
		cryptokernel/cryptokernel.h\
		cryptokernel/interval_set.h cryptokernel/interval_set.hpp
