// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordcontentwidget.h"

RecordContentWidget::RecordContentWidget(QWidget *parent):
	QFrame(parent),
	mainLayout_(new QVBoxLayout(this)),
	
	nameLineEdit_(new LineEditConfirm(this)),
	
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
	
	// Head
	this->nameLineEdit_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	auto nameLabelFont = this->nameLineEdit_->font();
	nameLabelFont.setWeight(QFont::Bold);
	nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
	this->nameLineEdit_->setFont(nameLabelFont);
	
	this->typeLabel_->setPalette(palette);
	this->typeLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
	this->typeContent_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	this->groupLabel_->setPalette(palette);
	this->groupLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
	this->groupContent_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	this->headLayout_->setContentsMargins(0, 0, 0, 0);
	//this->headLayout_->setVerticalSpacing(5);
	this->headLayout_->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->headLayout_->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	this->headLayout_->addRow(this->typeLabel_, this->typeContent_);
	this->headLayout_->addRow(this->groupLabel_, this->groupContent_);
	
	this->headWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->headWidget_->setLayout(this->headLayout_);
	
	this->hLine_->setFrameShape(QFrame::HLine);	// Horizontal line
	this->hLine_->setFrameShadow(QFrame::Sunken);
	
	// Form
	this->formLayout_->setContentsMargins(0, 5, 0, 0);
	this->formLayout_->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->formLayout_->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	this->scrollArea_->setLayout(this->formLayout_);
	this->scrollArea_->setFrameShape(QFrame::NoFrame);
	
	// Main
	this->mainLayout_->addWidget(this->nameLineEdit_);
	this->mainLayout_->addWidget(this->headWidget_);
	this->mainLayout_->addWidget(this->hLine_);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->setLayout(this->mainLayout_);
	this->setContentsMargins(5, 0, 5, 0);
	
	
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


QString RecordContentWidget::field(int index) const
{ return this->fields_[index].second->text(); }

QString RecordContentWidget::originalField(int index) const
{ return this->fields_[index].second->originalText(); }

void RecordContentWidget::setFields(const QList<QPair<QString, QString>> &fields)
{
	// Color for labels
	auto palette = this->palette();
	palette.setColor(QPalette::Foreground, Qt::gray);
	
	// Adding type labels and fields data
	int i = 0;
	for (const auto &field: fields) {
		auto label = new LabelButton(field.first, this);	// Label with text of first
		auto lineEdit = new LineEditConfirm(field.second, this);	// LineEdit with text of the second
		
		label->setPalette(palette);
		label->setMinimumHeight(lineEdit->height());
		
		this->connect(label, &LabelButton::clicked,
					  [this, i]() { this->onFieldClicked(i); });
		this->connect(lineEdit, &LineEditConfirm::accepted,
					  [this, i](QString newText) { this->onFieldChanged(i, newText); });
		++i;
		
		this->formLayout_->addRow(label, lineEdit);
		this->fields_.append(qMakePair(label, lineEdit));
	}
}


void RecordContentWidget::removeField(int index)
{
	auto &p = this->fields_.at(index);
	delete p.first;
	delete p.second;
	this->fields_.removeAt(index);
}

void RecordContentWidget::confirmFieldChanges(int index)
{ this->fields_[index].second->confirmText(); }


void RecordContentWidget::clear()
{
	this->hide();
	
	this->nameLineEdit_->setText("");
	this->groupContent_->setText("");
	this->typeContent_->setText("");
	
	for (auto &p: this->fields_) {
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


void RecordContentWidget::onFieldClicked(int index)
{ emit fieldClicked(index); }

void RecordContentWidget::onFieldChanged(int index, QString newText)
{ emit fieldChanged(index, newText); }
