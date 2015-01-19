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
	this->formLayout_->setVerticalSpacing(2);
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
				  this, &RecordContentWidget::onNameChanged);
	this->connect(this->typeLabel_, &LabelButton::clicked,
				  this, &RecordContentWidget::onTypeNameClicked);
	this->connect(this->groupLabel_, &LabelButton::clicked,
				  this, &RecordContentWidget::onGroupNameClicked);
}

RecordContentWidget::~RecordContentWidget()
{
	this->clear();
}


record_id_t RecordContentWidget::recordId() const
{ return this->recordId_; }

void RecordContentWidget::setRecordId(record_id_t id)
{ this->recordId_ = id; }


type_id_t RecordContentWidget::recordTypeId() const
{ return this->recordTypeId_; }

void RecordContentWidget::setRecordTypeId(type_id_t id)
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


void RecordContentWidget::addTypeField(tfield_id_t id, const QString &fieldName)
{
	this->typeFieldNames_[id] = fieldName;
	this->typeFieldsOrder_.push_back(id);
	for (auto &fieldPair: this->fields_)
		fieldPair.first->addItem(fieldName);
}


QString RecordContentWidget::field(rfield_id_t id) const
{ return this->fields_[id].second->text(); }

QString RecordContentWidget::originalField(rfield_id_t id) const
{ return this->fields_[id].second->originalText(); }


void RecordContentWidget::addField(rfield_id_t fieldId, tfield_id_t fieldTypeId, const QString &fieldData)
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
		++i;
	}
	comboBox->setFrame(false);
	
	this->connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
				  [this, fieldId](int fieldTypeIndex) { this->onFieldTypeChanged(fieldId, fieldTypeIndex); });
	this->connect(lineEdit, &LineEditConfirm::accepted,
				  [this, fieldId](QString newText) { this->onFieldChanged(fieldId, newText); });
	
	this->fields_[fieldId] = qMakePair(comboBox, lineEdit);
	this->formLayout_->addRow(comboBox, lineEdit);
}


void RecordContentWidget::removeField(rfield_id_t id)
{
	auto it = this->fields_.find(id);
	if (it != this->fields_.end()) {
		delete it->first;
		delete it->second;
		this->fields_.erase(it);
	}
}

void RecordContentWidget::confirmFieldChanges(rfield_id_t id)
{ this->fields_[id].second->confirmText(); }


void RecordContentWidget::clear()
{
	this->hide();
	
	this->nameLineEdit_->setText("");
	this->groupContent_->setText("");
	this->typeContent_->setText("");
	
	this->recordId_ = invalid_record_id;
	this->recordTypeId_ = invalid_type_id;
	
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
void RecordContentWidget::onNameChanged(QString newText)
{ emit nameChanged(newText); }

void RecordContentWidget::onTypeNameClicked()
{ emit typeNameClicked(); }

void RecordContentWidget::onGroupNameClicked()
{ emit groupNameClicked(); }


void RecordContentWidget::onFieldClicked(rfield_id_t id)
{ emit fieldClicked(id); }

void RecordContentWidget::onFieldChanged(rfield_id_t id, QString newText)
{ emit fieldChanged(id, newText); }

void RecordContentWidget::onFieldTypeChanged(rfield_id_t fieldId, tfield_id_t typeFieldIndex)
{
	auto fieldTypeId = this->typeFieldsOrder_.value(typeFieldIndex, invalid_tfield_id);
	if (fieldTypeId != invalid_tfield_id)
		emit fieldTypeChanged(fieldId, fieldTypeId);
}
