# Author: Dmitry Kukovinets (d1021976@gmail.com)

QT += core gui
QMAKE_MAC_SDK = macosx10.9
QMAKE_CXXFLAGS += -std=c++11 -Wall
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PM
TEMPLATE = app


SOURCES += main.cpp \
		mainwindow.cpp \
		taskwidget.cpp \
		tasklistwidget.cpp \
		leftpanelwidget.cpp \
		mainwidget.cpp \
		grouplistwidget.cpp \
		recordlistwidget.cpp \
		recordcontentwidget.cpp \
		settingssaver.cpp \
		abstractitem.cpp \
		groupitem.cpp \
		recorditem.cpp \
		typeitem.cpp \
		labelbutton.cpp \
		lineeditconfirm.cpp \
		warningwindow.cpp \
		cryptokernelagent.cpp cryptokernelagent_data.cpp cryptokernelagent_gui.cpp

HEADERS += types.h \
		mainwindow.h \
		taskwidget.h \
		tasklistwidget.h \
		leftpanelwidget.h \
		mainwidget.h \
		grouplistwidget.h \
		recordlistwidget.h \
		recordcontentwidget.h \
		settingssaver.h \
		abstractitem.h abstractitem.hpp \
		groupitem.h \
		recorditem.h \
		typeitem.h \
		labelbutton.h \
		lineeditconfirm.h \
		warningwindow.h \
		cryptokernelagent.h cryptokernelagent_data.hpp \
		algorithm.h algorithm.hpp

LIBS += cryptokernel/libcryptokernel.a
