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
	removeAction_(new QAction(tr("Remove"), this)),
	
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
	this->leftPanelWidget()->groupListWidget()->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->leftPanelWidget()->groupListWidget()->addAction(this->addRecordAction_);
	this->leftPanelWidget()->groupListWidget()->addAction(this->addGroupAction_);
	this->leftPanelWidget()->groupListWidget()->addAction(this->addTypeAction_);
	this->leftPanelWidget()->groupListWidget()->addAction(this->removeAction_);
	
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
	// Menu bar
	// Add group/record/type
	this->connect(this->addGroupAction_, &QAction::triggered,
				  this, &MainWindow::onAddGroupActionActivated);
	this->connect(this->addRecordAction_, &QAction::triggered,
				  this, &MainWindow::onAddRecordActionActivated);
	this->connect(this->addTypeAction_, &QAction::triggered,
				  this, &MainWindow::onAddTypeActionActivated);
	this->connect(this->removeAction_, &QAction::triggered,
				  this, &MainWindow::onRemoveActionActivated);
	
	// Groups list -> records list
	this->connect(this->leftPanelWidget()->groupListWidget(), &GroupListWidget::itemSelectionChanged,
				  this, &MainWindow::updateRecordListItems);
	
	// Records list -> record content widget
	this->connect(this->mainWidget()->recordListWidget(), &RecordListWidget::itemSelectionChanged,
				  this, &MainWindow::updateRecordContent);
	
	// Record content -> agent
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::nameClicked,
				  this, &MainWindow::onNameClicked);
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::nameChanged,
				  this, &MainWindow::onNameChanged);
	
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::typeClicked,
				  this, &MainWindow::onTypeClicked);
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::typeChanged,
				  this, &MainWindow::onTypeChanged);
	
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::groupClicked,
				  this, &MainWindow::onGroupClicked);
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::groupChanged,
				  this, &MainWindow::onGroupChanged);
	
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::fieldClicked,
				  this, &MainWindow::onFieldClicked);
	this->connect(this->mainWidget()->recordContentWidget(), &RecordContentWidget::fieldChanged,
				  this, &MainWindow::onFieldChanged);
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


bool MainWindow::needSaveSettings() const
{ return this->saveSettings_; }

void MainWindow::setSaveSettings(bool enable)
{ this->saveSettings_ = enable; }


LeftPanelWidget * MainWindow::leftPanelWidget() const
{ return this->leftPanelWidget_; }

MainWidget * MainWindow::mainWidget() const
{ return this->mainWidget_; }

CryptoKernelAgent * MainWindow::agent() const
{ return this->agent_; }


void MainWindow::updateRecordListItems()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_updateRecordListItems();
}


void MainWindow::updateRecordContent()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_updateRecordContent();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	if (this->needSaveSettings()) {
		QSettings settings;
		this->writeSettings(settings);
	}
	this->QMainWindow::closeEvent(event);
	
	if (this->agent_ != nullptr) this->agent_->GUI_onMainWindowClosed();
}


// Slots
void MainWindow::onNameClicked()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onNameClicked();
}

void MainWindow::onNameChanged(QString newName)
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onNameChanged(newName);
}

void MainWindow::onTypeClicked()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onTypeClicked();
}

void MainWindow::onTypeChanged(QString newTypeName)
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onTypeChanged(newTypeName);
}

void MainWindow::onGroupClicked()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onGroupClicked();
}

void MainWindow::onGroupChanged(QString newGroupName)
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onGroupChanged(newGroupName);
}

void MainWindow::onFieldClicked(int index)
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onFieldClicked(index);
}

void MainWindow::onFieldChanged(int index, QString newText)
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_onFieldChanged(index, newText);
}


void MainWindow::onAddGroupActionActivated()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_addGroup();
}

void MainWindow::onAddRecordActionActivated()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_addRecord();
}

void MainWindow::onAddTypeActionActivated()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_addType();
}

void MainWindow::onRemoveActionActivated()
{
	if (this->agent_ == nullptr) return;
	this->agent_->GUI_removeSelectedItems();
}
