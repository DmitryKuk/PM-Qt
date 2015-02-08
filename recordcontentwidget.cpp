// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordcontentwidget.h"

RecordContentWidget::RecordContentWidget(QWidget *parent):
	QFrame(parent),
	mainLayout_(new QVBoxLayout(this)),
	
	nameLineEdit_(new LineEditConfirm(this, true)),
	
	headWidget_(new QWidget(this)),
	headLayout_(new QFormLayout(this->headWidget_)),
	
	typeLabel_(new LabelButton(tr("Type"), this->headWidget_)),
	typeContent_(new QLabel(this->headWidget_)),
	
	groupLabel_(new LabelButton(tr("Group"), this->headWidget_)),
	groupContent_(new QLabel(this->headWidget_)),
	
	hLine_(new QFrame(this)),
	
	scrollArea_(new QScrollArea(this)),
	formLayout_(new QFormLayout(this->scrollArea_))
{
	auto palette = this->palette();
	palette.setColor(QPalette::Foreground, Qt::gray);
	
	this->typeLabel_->setPalette(palette);
	this->typeLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
	this->typeContent_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	this->groupLabel_->setPalette(palette);
	this->groupLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
	this->groupContent_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	this->headLayout_->setContentsMargins(0, 0, 0, 4);
	this->headLayout_->setVerticalSpacing(4);
	this->headLayout_->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->headLayout_->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	this->headLayout_->addRow(this->typeLabel_, this->typeContent_);
	this->headLayout_->addRow(this->groupLabel_, this->groupContent_);
	
	this->headWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->headWidget_->setLayout(this->headLayout_);
	
	this->hLine_->setFrameShape(QFrame::HLine);	// Horizontal line
	this->hLine_->setFrameShadow(QFrame::Sunken);
	
	// Form
	this->formLayout_->setVerticalSpacing(1);
	this->formLayout_->setContentsMargins(5, 5, 5, 5);
	this->formLayout_->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->formLayout_->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	this->scrollArea_->setLayout(this->formLayout_);
	this->scrollArea_->setFrameShape(QFrame::NoFrame);
	
	// Main
	this->mainLayout_->setSpacing(2);
	this->mainLayout_->setContentsMargins(10, 0, 10, 0);
	this->mainLayout_->addWidget(this->nameLineEdit_);
	this->mainLayout_->addWidget(this->headWidget_);
	this->mainLayout_->addWidget(this->hLine_);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->setLayout(this->mainLayout_);
	
	
	// Connections
	this->connect(this->nameLineEdit_, &LineEditConfirm::accepted,
				  [this](QString newText) { emit nameChanged(newText); });
	this->connect(this->typeLabel_, &LabelButton::clicked,
				  [this]() { emit typeNameClicked(); });
	this->connect(this->groupLabel_, &LabelButton::clicked,
				  [this]() { emit groupNameClicked(); });
}

RecordContentWidget::~RecordContentWidget()
{
	this->clear();
}


types::record_id RecordContentWidget::recordId() const
{ return this->recordId_; }

void RecordContentWidget::setRecordId(types::record_id id)
{ this->recordId_ = id; }


types::type_id RecordContentWidget::recordTypeId() const
{ return this->recordTypeId_; }

void RecordContentWidget::setRecordTypeId(types::type_id id)
{ this->recordTypeId_ = id; }


QString RecordContentWidget::name() const
{ return this->nameLineEdit_->text(); }

QString RecordContentWidget::origanalName() const
{ return this->nameLineEdit_->originalText(); }

void RecordContentWidget::setName(const QString &name)
{ this->nameLineEdit_->setText(name); }

void RecordContentWidget::confirmNameChanges()
{ this->nameLineEdit_->confirmText(); }


QString RecordContentWidget::groupName() const
{ return this->groupContent_->text(); }

void RecordContentWidget::setGroupName(const QString &groupName)
{ this->groupContent_->setText(groupName); }


QString RecordContentWidget::typeName() const
{ return this->typeContent_->text(); }

void RecordContentWidget::setTypeName(const QString &typeName)
{ this->typeContent_->setText(typeName); }


void RecordContentWidget::addTypeField(types::tfield_id id, const QString &fieldName)
{
	this->typeFieldNames_[id] = fieldName;
	this->typeFieldsOrder_.push_back(id);
	for (auto &fieldPair: this->fields_)
		fieldPair.first->addItem(fieldName);
}


QString RecordContentWidget::field(types::rfield_id id) const
{ return this->fields_[id].second->text(); }

QString RecordContentWidget::originalField(types::rfield_id id) const
{ return this->fields_[id].second->originalText(); }


void RecordContentWidget::addField(types::rfield_id fieldId, types::tfield_id fieldTypeId, const QString &fieldData)
{
	auto comboBox = new QComboBox(this->scrollArea_);
	auto lineEdit = new LineEditConfirm(fieldData, this);
	
	// Color for comboBox
	// auto palette = this->palette();
	// palette.setColor(QPalette::Foreground, Qt::gray);
	// comboBox->setPalette(palette);
	// comboBox->setMinimumHeight(lineEdit->height());
	
	// Filling combo box
	int i = 0;
	for (auto typeFieldId: this->typeFieldsOrder_) {
		comboBox->addItem(this->typeFieldNames_[typeFieldId]);
		if (typeFieldId == fieldTypeId)
			comboBox->setCurrentIndex(i);
		
		comboBox->setItemData(i, Qt::AlignRight, Qt::TextAlignmentRole);
		++i;
	}
	comboBox->setFrame(false);
	
	// Connections
	this->connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
				  [this, fieldId](int typeFieldIndex) { this->onFieldTypeChanged(fieldId, typeFieldIndex); });
	this->connect(lineEdit, &LineEditConfirm::accepted,
				  [this, fieldId](QString newText) { this->onFieldChanged(fieldId, newText); });
	
	this->fields_[fieldId] = qMakePair(comboBox, lineEdit);
	this->formLayout_->addRow(comboBox, lineEdit);
}


void RecordContentWidget::removeField(types::rfield_id id)
{
	auto it = this->fields_.find(id);
	if (it != this->fields_.end()) {
		delete it->first;
		delete it->second;
		this->fields_.erase(it);
	}
}

void RecordContentWidget::confirmFieldChanges(types::rfield_id id)
{ this->fields_[id].second->confirmText(); }


void RecordContentWidget::clear()
{
	this->hide();
	
	this->nameLineEdit_->setText("");
	this->groupContent_->setText("");
	this->typeContent_->setText("");
	
	this->recordId_ = types::record_id::invalid();
	this->recordTypeId_ = types::type_id::invalid();
	
	this->typeFieldNames_.clear();
	this->typeFieldsOrder_.clear();
	
	for (auto &p: this->fields_) {
		p.first->clear();
		delete p.first;
		delete p.second;
	}
	this->fields_.clear();
}


void RecordContentWidget::readSettings(QSettings &settings, const QString &prefix)
{
	QString current_prefix = prefix + "/recordContentWidget";
	
	// This widget
	this->restoreGeometry(settings.value(current_prefix + "/geometry").toByteArray());
}

void RecordContentWidget::writeSettings(QSettings &settings, const QString &prefix) const
{
	QString current_prefix = prefix + "/recordContentWidget";
	
	// This widget
	settings.setValue(current_prefix + "/geometry", this->saveGeometry());
}


// Slots

void RecordContentWidget::onFieldClicked(types::rfield_id id)
{ emit fieldClicked(id); }

void RecordContentWidget::onFieldChanged(types::rfield_id id, QString newText)
{ emit fieldChanged(id, newText); }

void RecordContentWidget::onFieldTypeChanged(types::rfield_id fieldId, int typeFieldIndex)
{
	auto typeFieldId = this->typeFieldsOrder_.value(typeFieldIndex, types::tfield_id::invalid());
	if (typeFieldId.is_valid())
		emit fieldTypeChanged(fieldId, typeFieldId);
}
