// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordcontentwidget.h"

RecordContentWidget::RecordContentWidget(QWidget *parent):
	QFrame(parent),
	mainLayout_(new QVBoxLayout(this)),
	
	nameLabel_(new LabelButton(this)),
	
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
	this->nameLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	auto nameLabelFont = this->nameLabel_->font();
	nameLabelFont.setWeight(QFont::Bold);
	nameLabelFont.setPointSize(nameLabelFont.pointSize() + 4);
	this->nameLabel_->setFont(nameLabelFont);
	
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
	this->mainLayout_->addWidget(this->nameLabel_);
	this->mainLayout_->addWidget(this->headWidget_);
	this->mainLayout_->addWidget(this->hLine_);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->setLayout(this->mainLayout_);
	this->setContentsMargins(5, 0, 5, 0);
	
	
	// Connections
	this->connect(this->nameLabel_, &LabelButton::clicked,
				  this, &RecordContentWidget::onNameClicked);
	this->connect(this->typeLabel_, &LabelButton::clicked,
				  this, &RecordContentWidget::onTypeClicked);
	this->connect(this->groupLabel_, &LabelButton::clicked,
				  this, &RecordContentWidget::onGroupClicked);
}

RecordContentWidget::~RecordContentWidget()
{
	this->clear();
}


QString RecordContentWidget::name() const
{ return this->nameLabel_->text(); }

void RecordContentWidget::setName(const QString &name)
{ this->nameLabel_->setText(name); }


QString RecordContentWidget::group() const
{ return this->groupContent_->text(); }

void RecordContentWidget::setGroup(const QString &groupName)
{ this->groupContent_->setText(groupName); }


QString RecordContentWidget::type() const
{ return this->typeContent_->text(); }

void RecordContentWidget::setType(const QString &typeName)
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
	for (const auto &field: fields) {
		auto label = new LabelButton(field.first, this);	// Label with text of first
		auto lineEdit = new LineEditConfirm(field.second, this);	// LineEdit with text of the second
		
		label->setPalette(palette);
		
		this->connect(label, &LabelButton::clicked,
					  this, &RecordContentWidget::onFieldClicked);
		this->connect(lineEdit, &LineEditConfirm::confirmButtonClicked,
					  this, &RecordContentWidget::onFieldChanged);
		
		this->formLayout_->addRow(label, lineEdit);
		this->fields_.append(qMakePair(label, lineEdit));
	}
}


void RecordContentWidget::clear()
{
	this->hide();
	
	this->nameLabel_->setText("");
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
void RecordContentWidget::onNameClicked(LabelButton *button)
{
	if (button == this->nameLabel_)
		emit nameClicked();
}

void RecordContentWidget::onNameChanged(QString newText)
{
	emit nameChanged(newText);
}

void RecordContentWidget::onTypeClicked(LabelButton *button)
{
	if (button == this->typeLabel_)
		emit typeClicked();
}

void RecordContentWidget::onTypeChanged(QString newText)
{
	emit typeChanged(newText);
}

void RecordContentWidget::onGroupClicked(LabelButton *button)
{
	if (button == this->groupLabel_)
		emit groupClicked();
}

void RecordContentWidget::onGroupChanged(QString newText)
{
	emit groupChanged(newText);
}

void RecordContentWidget::onFieldClicked(LabelButton *button)
{
	int i = 0;
	for (const auto &p: this->fields_) {
		if (button == p.first) {
			emit fieldClicked(i);
			break;
		}
		++i;
	}
}

void RecordContentWidget::onFieldChanged(LineEditConfirm *field, QString newText)
{
	int i = 0;
	for (const auto &p: this->fields_) {
		if (field == p.second) {
			emit fieldChanged(i, newText);
			break;
		}
		++i;
	}
}
