/***************************************************
** Author: Dmitry Kukovinets (d1021976@gmail.com) **
***************************************************/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
	QMainWindow(parent)
{
	// Test window
	// QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"));
	
	// QWidget *label = new QLabel(tr("Hello, world!"));
	// QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
	// QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
	// QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));
	
	// //radio1->setChecked(true);
	
	// QVBoxLayout *vbox = new QVBoxLayout;
	// vbox->addWidget(label);
	// vbox->addWidget(radio1);
	// vbox->addWidget(radio2);
	// vbox->addWidget(radio3);
	// //vbox->addStretch(1);
	// groupBox->setLayout(vbox);
	// QMainWindow::setCentralWidget(groupBox);
	
	
	// .ui file usage
	//Ui::MainWindow mainWindowUi;
	//mainWindowUi.setupUi(this);
	
	
	// Example from stackoverflow
	// QWidget wnd;
	
	// QTextEdit *editor1 = new QTextEdit;
	// QTextEdit *editor2 = new QTextEdit;
	// QTextEdit *editor3 = new QTextEdit;
	
	// QSplitter *split1 = new QSplitter;
	// QSplitter *split2 = new QSplitter;
	
	// QVBoxLayout *layout = new QVBoxLayout;
	
	// QWidget *container = new QWidget;
	// QVBoxLayout *container_layout = new QVBoxLayout;
	
	// split1->addWidget(editor1);
	// split1->addWidget(editor2);
	
	// container_layout->addWidget(split1);
	// container->setLayout(container_layout);
	
	// split2->setOrientation(Qt::Vertical);
	// split2->addWidget(container);
	// split2->addWidget(editor3);
	
	// layout->addWidget(split2);
	
	// wnd.setLayout(layout);
	
	
	// QSplitter *mainSplit = new QSplitter;
	// mainSplit->addWidget(new QTextEdit);
	// mainSplit->addWidget(new QTextEdit);
	// mainSplit->addWidget(new QTextEdit);
	// QMainWindow::setCentralWidget(mainSplit);
	
	QMainWindow::setCentralWidget(new Task("Some widget"));
}

MainWindow::~MainWindow()
{}
