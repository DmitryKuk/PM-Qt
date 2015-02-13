// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "mainwindow.h"

#include "cryptokernelagent.h"

MainWindow::MainWindow(CryptoKernelAgent *agent,
					   const QString &title,
					   bool autoLoadSettings,
					   bool autoSaveSettings,
					   QWidget *parent):
	QMainWindow(parent),	// Parent
	agent_(agent),			// Agent
	
	// Context menu
	addGroupAction_(new QAction(tr("Add group"), this)),
	addRecordAction_(new QAction(tr("Add record"), this)),
	addTypeAction_(new QAction(tr("Add type"), this)),
	removeAction_(new QAction(tr("Remove selected"), this)),
	editTypeAction_(new QAction(tr("Edit type"), this)),
	addRecordFieldAction_(new QAction(tr("Add field"), this)),
	
	mainSplit_(new QSplitter(this)),
	leftPanelWidget_(new LeftPanelWidget(mainSplit_)),
	mainWidget_(new MainWidget(mainSplit_)),
	
	saveSettings_(autoSaveSettings? true: false)	// Other
{
	if (autoLoadSettings) {
		QSettings settings;
		this->readSettings(settings);
	}
	
	// Context menu
	// Group list
	this->groupListWidget()->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->groupListWidget()->addAction(this->addRecordAction_);
	this->groupListWidget()->addAction(this->addGroupAction_);
	this->groupListWidget()->addAction(this->addTypeAction_);
	this->groupListWidget()->addAction(this->removeAction_);
	this->groupListWidget()->addAction(this->editTypeAction_);
	
	// Record content
	this->recordContentWidget()->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->recordContentWidget()->addAction(this->addRecordFieldAction_);
	
	// Main splitter setting...
	this->mainSplit_->setOrientation(Qt::Horizontal);
	this->mainSplit_->addWidget(this->leftPanelWidget_);
	this->mainSplit_->addWidget(this->mainWidget_);
	this->mainSplit_->setCollapsible(0, false);
	this->mainSplit_->setCollapsible(1, false);
	this->mainSplit_->setHandleWidth(0);
	
	// Main setting...
	this->setWindowTitle(title);
	this->setCentralWidget(this->mainSplit_);
	
	// Background color
	auto palette = this->palette();
	palette.setColor(QPalette::Background, Qt::white);
	this->setPalette(palette);
	
	
	// Connections
	// Add group/record/type, remove selected, edit type
	this->connect(this->addGroupAction_,  &QAction::triggered, this->agent_, &CryptoKernelAgent::GUI_addGroup);
	this->connect(this->addRecordAction_, &QAction::triggered, this->agent_, &CryptoKernelAgent::GUI_addRecord);
	this->connect(this->addTypeAction_,   &QAction::triggered, this->agent_, &CryptoKernelAgent::GUI_addType);
	this->connect(this->removeAction_,    &QAction::triggered, this->agent_, &CryptoKernelAgent::GUI_removeSelectedItems);
	this->connect(this->editTypeAction_,  &QAction::triggered, this->agent_, &CryptoKernelAgent::GUI_showTypeEditDialog);
	
	// Groups list -> records list
	this->connect(this->groupListWidget(), &GroupListWidget::itemSelectionChanged,		this->agent_, &CryptoKernelAgent::GUI_updateRecordListItems);
	
	// Records list -> record content widget
	this->connect(this->recordListWidget(), &RecordListWidget::itemSelectionChanged,	this->agent_, &CryptoKernelAgent::GUI_updateRecordContent);
	
	// Record content -> agent
	this->connect(this->addRecordFieldAction_, &QAction::triggered,						this->agent_, &CryptoKernelAgent::GUI_addRecordField);
	
	this->connect(this->recordContentWidget(), &RecordContentWidget::nameChanged,		this->agent_, &CryptoKernelAgent::GUI_onRecordNameChanged);
	this->connect(this->recordContentWidget(), &RecordContentWidget::typeNameClicked,	this->agent_, &CryptoKernelAgent::GUI_onRecordTypeNameClicked);
	this->connect(this->recordContentWidget(), &RecordContentWidget::groupNameClicked,	this->agent_, &CryptoKernelAgent::GUI_onRecordGroupNameClicked);
	
	this->connect(this->recordContentWidget(), &RecordContentWidget::fieldTypeChanged,	this->agent_, &CryptoKernelAgent::GUI_setRecordFieldType);
	this->connect(this->recordContentWidget(), &RecordContentWidget::fieldClicked,		this->agent_, &CryptoKernelAgent::GUI_onRecordFieldClicked);
	this->connect(this->recordContentWidget(), &RecordContentWidget::fieldChanged,		this->agent_, &CryptoKernelAgent::GUI_onRecordFieldChanged);
	
	// Group list: items names
	this->connect(this->groupListWidget(), &GroupListWidget::itemChanged,				this->agent_, &CryptoKernelAgent::GUI_onItemDataChanged);
}


void MainWindow::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/mainWindow";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
	this->restoreState(settings.value(current_prefix + "/state").toByteArray());
	
	// Child widgets
	this->mainSplit_->restoreGeometry(settings.value(current_prefix + "/mainSplit/geometry").toByteArray());
	this->mainSplit_->restoreState(settings.value(current_prefix + "/mainSplit/state").toByteArray());
	
	this->leftPanelWidget_->readSettings(settings, current_prefix);
	this->mainWidget_->readSettings(settings, current_prefix);
}

void MainWindow::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/mainWindow";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
	settings.setValue(current_prefix + "/state", this->saveState());
	
	// Child widgets
	settings.setValue(current_prefix + "/mainSplit/geometry", this->mainSplit_->saveGeometry());
	settings.setValue(current_prefix + "/mainSplit/state", this->mainSplit_->saveState());
	
	this->leftPanelWidget_->writeSettings(settings, current_prefix);
	this->mainWidget_->writeSettings(settings, current_prefix);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	if (this->needSaveSettings()) {
		QSettings settings;
		this->writeSettings(settings);
	}
	this->QMainWindow::closeEvent(event);
}
