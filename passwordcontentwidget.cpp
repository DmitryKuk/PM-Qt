// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "passwordcontentwidget.h"

PasswordContentWidget::PasswordContentWidget(const QString &label, QWidget *parent):
	QGroupBox(parent),
	label_(new QLabel(this))
{
	this->label_->setText(label);
}
