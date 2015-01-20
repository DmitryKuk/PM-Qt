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
	
	QString field(tfield_id_t id) const;
	QString originalField(tfield_id_t id) const;
	void addField(tfield_id_t id, const QString &text);
	void removeField(tfield_id_t id);
	void confirmFieldChanges(tfield_id_t id);
	
	void clear();
signals:
	void nameChanged(QString newName);
	void fieldChanged(tfield_id_t id, QString newText);
	void fieldAdded();
private:
	// Slots
	void onNameChanged(QString newText);
	void onFieldChanged(tfield_id_t id, QString newText);
	void onAddFieldActionActivated();
	
	
	LineEditConfirm *nameLineEdit_;
	QScrollArea *scrollArea_;
	QDialogButtonBox *buttonBox_;
	QVBoxLayout *mainLayout_, *scrollAreaLayout_;
	
	QMap<tfield_id_t, LineEditConfirm *> fields_;
	
	// Context menu
	QAction *addTypeFieldAction_;
};

#endif // TYPEEDITDIALOG_H
