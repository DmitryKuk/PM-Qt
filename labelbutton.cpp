// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "labelbutton.h"

LabelButton::LabelButton(QWidget *parent):
	QLabel(parent)
{}

LabelButton::LabelButton(const QString &text, QWidget *parent):
	QLabel(text, parent)
{}


void LabelButton::mouseReleaseEvent(QMouseEvent *ev)
{
	this->QLabel::mouseReleaseEvent(ev);
	emit clicked(this);
}
