// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordlistwidget.h"

PasswordListWidget::PasswordListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this))
{
	this->label_->setText(label);
}
