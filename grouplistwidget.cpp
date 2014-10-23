// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "grouplistwidget.h"

GroupListWidget::GroupListWidget(const QString &label, QWidget *parent):
	QFrame(parent),
	label_(new QLabel(this))
{
	this->label_->setText(label);
}
