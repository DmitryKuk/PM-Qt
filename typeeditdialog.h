// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef TYPEEDITDIALOG_H
#define TYPEEDITDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QAction>

#include "lineeditconfirm.h"

#include <QList>

class TypeEditDialog: public QDialog
{
	Q_OBJECT
public:
	TypeEditDialog(QWidget *parent = nullptr);
	
	QString name() const;
	void setName(const QString &name);
	void confirmNameChanges();
	
	QString field(int index) const;
	QString originalField(int index) const;
	void setFields(const QList<QString> &fields);
	void addField(const QString &text);
	
	void removeField(int index);
	void confirmFieldChanges(int index);
	
	void clear();
signals:
	void nameChanged(QString newName);
	void fieldChanged(int index, QString newText);
	void fieldAdded();
private:
	// Slots
	void onNameChanged(QString newText);
	void onFieldChanged(int index, QString newText);
	void onAddFieldActionActivated();
	
	
	LineEditConfirm *nameLineEdit_;
	QScrollArea *scrollArea_;
	QDialogButtonBox *buttonBox_;
	QVBoxLayout *mainLayout_, *scrollAreaLayout_;
	
	QList<LineEditConfirm *> fields_;
	
	// Context menu
	QAction *addTypeFieldAction_;
};

#endif // TYPEEDITDIALOG_H
