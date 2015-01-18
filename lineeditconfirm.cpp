// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "lineeditconfirm.h"


LineEditConfirm::LineEditConfirm(QWidget *parent, bool isTitle):
	QFrame(parent),
	layout_(new QHBoxLayout(this)),
	lineEdit_(new QLineEdit(this)),
	buttonBox_(new QDialogButtonBox(this)),
	originalText_("")
{
	this->init(isTitle);
}

LineEditConfirm::LineEditConfirm(const QString &title, QWidget *parent, bool isTitle):
	QFrame(parent),
	layout_(new QHBoxLayout(this)),
	lineEdit_(new QLineEdit(title, this)),
	buttonBox_(new QDialogButtonBox(this)),
	originalText_(title)
{
	this->init(isTitle);
}


void LineEditConfirm::setText(const QString &text)
{
	this->lineEdit_->setText(text);
	this->originalText_ = text;
	this->hideButtons();
}

QString LineEditConfirm::text() const
{ return this->lineEdit_->text(); }

QString LineEditConfirm::originalText() const
{ return this->originalText_; }


void LineEditConfirm::setFrame(bool status)
{
	if (status) this->setFrameShape(QFrame::Box);
	else this->setFrameShape(QFrame::NoFrame);
}


// Slots
void LineEditConfirm::confirmText()
{
	this->originalText_ = this->lineEdit_->text();
	this->hideButtons();
}

void LineEditConfirm::restoreText()
{
	this->lineEdit_->setText(this->originalText_);
	this->hideButtons();
}


void LineEditConfirm::init(bool isTitle)
{
	if (isTitle) {
		auto lineEditFont = this->lineEdit_->font();
		lineEditFont.setWeight(QFont::Bold);
		lineEditFont.setPointSize(lineEditFont.pointSize() + 4);
		this->lineEdit_->setFont(lineEditFont);
	}
	
	this->lineEdit_->setFrame(false);
	this->setFrameShadow(QFrame::Sunken);
	
	this->buttonBox_->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
	this->buttonBox_->button(QDialogButtonBox::Save)->setDefault(true);
	
	this->layout_->setContentsMargins(7, 7, 5, 0);
	this->layout_->addWidget(this->lineEdit_);
	this->layout_->addWidget(this->buttonBox_);
	this->setLayout(this->layout_);
	this->hideButtons();
	
	//this->setContentsMargins(1, 1, 1, 1);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	// Connections
	this->connect(this->lineEdit_, &QLineEdit::textEdited,
				  this, &LineEditConfirm::onTextEdited);
	this->connect(this->buttonBox_, &QDialogButtonBox::accepted,
				  this, &LineEditConfirm::onAccepted);
	this->connect(this->buttonBox_, &QDialogButtonBox::rejected,
				  this, &LineEditConfirm::onRejected);
}


void LineEditConfirm::showButtons()
{
	this->buttonBox_->show();
	this->setFrame(true);
}

void LineEditConfirm::hideButtons()
{
	this->buttonBox_->hide();
	this->setFrame(false);
}


// Slots
void LineEditConfirm::onAccepted()
{ emit accepted(this->lineEdit_->text()); }

void LineEditConfirm::onRejected()
{
	this->restoreText();
	emit rejected();
}


void LineEditConfirm::onTextEdited(const QString &newText)
{
	if (newText == this->originalText_) this->hideButtons();
	else this->showButtons();
}
