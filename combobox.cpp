// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "combobox.h"

ComboBox::ComboBox(QWidget *parent):
	QComboBox(parent)
{
	this->connect(this, static_cast<void (ComboBox::*)(int)>(&ComboBox::currentIndexChanged),
				  this, &ComboBox::onCurrentIndexChanged);
}


void ComboBox::onCurrentIndexChanged(int newIndex)
{ emit changed(newIndex); }
