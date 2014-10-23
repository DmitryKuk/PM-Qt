// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwindow.h"

MainWindow::MainWindow(const QString &title, QWidget *parent):
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
	
	// TaskWidget's
	// auto *widget = new QWidget(this);
	// QVBoxLayout *layout = new QVBoxLayout(widget);
	// widget->setLayout(layout);
	
	// auto *widget1 = new TaskWidget("Some task 1", widget);
	// auto *widget2 = new TaskWidget("Some task 2", widget);
	// auto *widget3 = new TaskWidget("Some task 3", widget);
	
	// layout->addWidget(widget1);
	// layout->addWidget(widget2);
	// layout->addWidget(widget3);
	
	// widget1->setProgress(50);
	// widget2->setProgress(30);
	// widget3->setProgress(75);
	
	
	// TaskListWidget
	// auto *widget = new TaskListWidget("Tasks", this);
	// widget->addTask("First task");
	// widget->addTask("Second task");
	
	// Left panel widget
	auto *widget = new LeftPanelWidget(this);
	auto *taskListWidget = widget->taskListWidget();
	
	auto it1 = taskListWidget->addTask("Some task 1");
	auto it2 = taskListWidget->addTask("Some task 2");
	auto it3 = taskListWidget->addTask("Some task 3");
	
	(*it1)->setProgress(50);
	(*it2)->setProgress(30);
	(*it3)->setProgress(75);
	
	this->setWindowTitle(title);
	QMainWindow::setCentralWidget(widget);
}

MainWindow::~MainWindow()
{}
