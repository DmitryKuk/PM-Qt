// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "addrecordfielddialog.h"

AddRecordFieldDialog::AddRecordFieldDialog(QWidget *parent):
	typeFieldLabel_(new QLabel(this)),
	dataFieldLabel_(new QLabel(this)),
	typeFieldComboBox_(new QComboBox(this)),
	dataFieldLineEdit_(new QLineEdit(this)),
	buttonBox_(new QDialogButtonBox(this)),
	fieldsLayout_(new QFormLayout),
	mainLayout_(new QVBoxLayout(this))
{
	
}
