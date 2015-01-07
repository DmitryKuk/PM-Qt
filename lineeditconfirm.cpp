// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "lineeditconfirm.h"

QIcon
	LineEditConfirm::confirmIcon_	= QIcon::fromTheme("list-add"),
	LineEditConfirm::cancelIcon_	= QIcon::fromTheme("edit-delete");

LineEditConfirm::LineEditConfirm(QWidget *parent):
	QFrame(parent),
	layout_(new QHBoxLayout(this)),
	lineEdit_(new QLineEdit(this)),
	confirmButton_(new QPushButton(LineEditConfirm::confirmIcon_, tr("OK"), this)),
	cancelButton_(new QPushButton(LineEditConfirm::cancelIcon_, tr("Cancel"), this))
{
	this->lineEdit_->setFrame(false);
	
	this->layout_->addWidget(this->lineEdit_);
	this->layout_->addWidget(this->confirmButton_);
	this->layout_->addWidget(this->cancelButton_);
	this->setLayout(this->layout_);
	this->hideButtons();
	
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	// Connections
	this->connect(this->lineEdit_, &QLineEdit::textEdited,
				  this, &LineEditConfirm::onTextEdited);
	this->connect(this->confirmButton_, &QPushButton::clicked,
				  this, &LineEditConfirm::onConfirmButtonClicked);
	this->connect(this->cancelButton_, &QPushButton::clicked,
				  this, &LineEditConfirm::onCancelButtonClicked);
}

LineEditConfirm::LineEditConfirm(const QString &title, QWidget *parent):
	QFrame(parent),
	layout_(new QHBoxLayout(this)),
	lineEdit_(new QLineEdit(title, this)),
	confirmButton_(new QPushButton(LineEditConfirm::confirmIcon_, tr("OK"), this)),
	cancelButton_(new QPushButton(LineEditConfirm::cancelIcon_, tr("Cancel"), this)),
	originalText_(title)
{
	this->lineEdit_->setFrame(false);
	
	this->layout_->addWidget(this->lineEdit_);
	this->layout_->addWidget(this->confirmButton_);
	this->layout_->addWidget(this->cancelButton_);
	this->setLayout(this->layout_);
	this->hideButtons();
	
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	
	// Connections
	this->connect(this->lineEdit_, &QLineEdit::textEdited,
				  this, &LineEditConfirm::onTextEdited);
	this->connect(this->confirmButton_, &QPushButton::clicked,
				  this, &LineEditConfirm::onConfirmButtonClicked);
	this->connect(this->cancelButton_, &QPushButton::clicked,
				  this, &LineEditConfirm::onCancelButtonClicked);
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


void LineEditConfirm::showButtons()
{
	this->confirmButton_->show();
	this->cancelButton_->show();
	this->setFrame(true);
}

void LineEditConfirm::hideButtons()
{
	this->confirmButton_->hide();
	this->cancelButton_->hide();
	this->setFrame(false);
}


// Slots
void LineEditConfirm::onConfirmButtonClicked()
{
	emit confirmButtonClicked(this, this->lineEdit_->text());
}

void LineEditConfirm::onCancelButtonClicked()
{
	this->restoreText();
}


void LineEditConfirm::onTextEdited(const QString &newText)
{
	if (newText == this->originalText_) this->hideButtons();
	else this->showButtons();
}
