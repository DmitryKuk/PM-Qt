// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "typeeditdialog.h"

TypeEditDialog::TypeEditDialog(QWidget *parent):
	QDialog(parent),
	
	nameLineEdit_(new LineEditConfirm(this, true)),
	scrollArea_(new QScrollArea(this)),
	buttonBox_(new QDialogButtonBox(this)),
	mainLayout_(new QVBoxLayout(this)),
	scrollAreaLayout_(new QVBoxLayout(scrollArea_)),
	
	addTypeFieldAction_(new QAction(tr("Add field"), this))	// Context menu
{
	this->setWindowTitle(tr("Type Editor"));
	this->addAction(this->addTypeFieldAction_);
	this->setContextMenuPolicy(Qt::ActionsContextMenu);
	
	auto palette = this->palette();
	palette.setColor(QPalette::Background, Qt::white);
	this->setPalette(palette);
	
	this->scrollAreaLayout_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	this->scrollAreaLayout_->setSpacing(1);
	this->scrollAreaLayout_->setContentsMargins(2, 2, 1, 2);
	this->scrollArea_->setLayout(this->scrollAreaLayout_);
	this->buttonBox_->setOrientation(Qt::Horizontal);
	this->buttonBox_->setStandardButtons(QDialogButtonBox::Close);
	
	this->mainLayout_->setContentsMargins(5, 5, 5, 5);
	this->mainLayout_->addWidget(this->nameLineEdit_);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->mainLayout_->addWidget(this->buttonBox_);
	
	this->setModal(true);
	this->setMinimumWidth(400);
	
	// Connections
	this->connect(this->addTypeFieldAction_, &QAction::triggered,
				  this, &TypeEditDialog::onAddFieldActionActivated);
	this->connect(this->nameLineEdit_, &LineEditConfirm::accepted,
				  this, &TypeEditDialog::onNameChanged);
	this->connect(this->buttonBox_, &QDialogButtonBox::clicked,
				  [this](QAbstractButton *) { this->close(); });
}


QString TypeEditDialog::name() const
{ return this->nameLineEdit_->text(); }

void TypeEditDialog::setName(const QString &name)
{ this->nameLineEdit_->setText(name); }

void TypeEditDialog::confirmNameChanges()
{ this->nameLineEdit_->confirmText(); }


QString TypeEditDialog::field(types::tfield_id id) const
{
	auto it = this->fields_.find(id);
	if (it == this->fields_.end()) return "";
	return (*it)->text();
}

QString TypeEditDialog::originalField(types::tfield_id id) const
{
	auto it = this->fields_.find(id);
	if (it == this->fields_.end()) return "";
	return (*it)->originalText();
}

void TypeEditDialog::addField(types::tfield_id id, const QString &text)
{
	auto lineEdit = new LineEditConfirm(text, this->scrollArea_);	// LineEdit with text of the second
	this->scrollAreaLayout_->addWidget(lineEdit);
	this->fields_[id] = lineEdit;
	
	lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	this->connect(lineEdit, &LineEditConfirm::accepted,
				  [this, id](QString newText) { this->onFieldChanged(id, newText); });
}

void TypeEditDialog::removeField(types::tfield_id id)
{
	auto it = this->fields_.find(id);
	if (it == this->fields_.end()) return;
	delete *it;
	this->fields_.erase(it);
}

void TypeEditDialog::confirmFieldChanges(types::tfield_id id)
{
	auto it = this->fields_.find(id);
	if (it == this->fields_.end()) return;
	(*it)->confirmText();
}


void TypeEditDialog::clear()
{
	this->close();
	
	this->nameLineEdit_->setText("");
	for (auto &lineEdit: this->fields_)
		delete lineEdit;
	this->fields_.clear();
}


// Slots
void TypeEditDialog::onNameChanged(QString newText)
{ emit nameChanged(newText); }


void TypeEditDialog::onFieldChanged(types::tfield_id id, QString newText)
{ emit fieldChanged(id, newText); }

void TypeEditDialog::onAddFieldActionActivated()
{ emit fieldAdded(); }
