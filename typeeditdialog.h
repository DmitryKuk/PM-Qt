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

#include <QMap>

#include "lineeditconfirm.h"
#include "types.h"

class TypeEditDialog: public QDialog
{
	Q_OBJECT
public:
	TypeEditDialog(QWidget *parent = nullptr);
	
	QString name() const;
	void setName(const QString &name);
	void confirmNameChanges();
	
	QString field(types::tfield_id id) const;
	QString originalField(types::tfield_id id) const;
	void addField(types::tfield_id id, const QString &text);
	void removeField(types::tfield_id id);
	void confirmFieldChanges(types::tfield_id id);
	
	void clear();
signals:
	void nameChanged(QString newName);
	void fieldChanged(types::tfield_id id, QString newText);
	void fieldAdded();
private:
	// Slots
	void onNameChanged(QString newText);
	void onFieldChanged(types::tfield_id id, QString newText);
	void onAddFieldActionActivated();
	
	
	LineEditConfirm *nameLineEdit_;
	QScrollArea *scrollArea_;
	QDialogButtonBox *buttonBox_;
	QVBoxLayout *mainLayout_, *scrollAreaLayout_;
	
	QMap<types::tfield_id, LineEditConfirm *> fields_;
	
	// Context menu
	QAction *addTypeFieldAction_;
};

#endif // TYPEEDITDIALOG_H
