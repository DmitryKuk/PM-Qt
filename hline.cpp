// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "hline.h"

HLine::HLine(QWidget *parent = nullptr):
	QFrame(parent)
{
	this->setFrameShadow(QFrame::Sunken);
	this->setLineWidth(1);
	this->setMinimumSize(0, 0);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
