// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "recordcontentwidget.h"

RecordContentWidget::RecordContentWidget(QWidget *parent):
	QGroupBox(tr("Password content"), parent),
	mainLayout_(new QVBoxLayout(this)),
	scrollArea_(new QScrollArea(this)),
	formLayout_(new QFormLayout(this->scrollArea_))
{
	// Form layout
	this->formLayout_->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
	this->scrollArea_->setLayout(this->formLayout_);
	this->scrollArea_->setFrameShape(QFrame::NoFrame);
	
	// Main layout
	this->mainLayout_->setContentsMargins(0, 0, 0, 0);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->setLayout(this->mainLayout_);
}

RecordContentWidget::~RecordContentWidget()
{
	this->clearFields();
}


void RecordContentWidget::setFields(const QList<QPair<QString, QString>> &fields)
{
	this->clearFields();
	
	// Adding type buttons and fields data
	for (const auto &field: fields) {
		// Button with text of first
		auto button = new QPushButton(field.first, this);
		//button->setFlat(true);
		
		// LineEdit with text of the second
		auto lineEdit = new QLineEdit(field.second, this);
		
		this->formLayout_->addRow(button, lineEdit);
		this->buttonEditMap_[button] = lineEdit;
	}
}


void RecordContentWidget::clearFields()
{
	for (auto &p: this->buttonEditMap_) {
		delete p.first;
		delete p.second;
	}
	this->buttonEditMap_.clear();
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
