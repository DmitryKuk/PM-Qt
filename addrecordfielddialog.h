// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef ADDRECORDFIELDDIALOG_H
#define ADDRECORDFIELDDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>

#include <QList>
#include <QString>

class AddRecordFieldDialog: public QDialog
{
	Q_OBJECT
public:
	AddRecordFieldDialog(QWidget *parent = nullptr);
	
	
signals:
	void accepted(int index, QString newField);
	void rejected();
private:
	QLabel *typeFieldLabel_, *dataFieldLabel_;
	QComboBox *typeFieldComboBox_;
	QLineEdit *dataFieldLineEdit_;
	QDialogButtonBox *buttonBox_;
	
	QFormLayout *fieldsLayout_;
	QVBoxLayout *mainLayout_;
};

#endif // ADDRECORDFIELDDIALOG_H
