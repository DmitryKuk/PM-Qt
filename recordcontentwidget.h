// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QMap>
#include <QList>
#include <QPair>
#include <QString>

#include "types.h"
#include "labelbutton.h"
#include "lineeditconfirm.h"
#include "settingssaver.h"

class RecordContentWidget:
	public QFrame,
	public SettingsSaver
{
	Q_OBJECT
public:
	RecordContentWidget(QWidget *parent = nullptr);
	~RecordContentWidget();
	
	types::record_id recordId() const;
	void setRecordId(types::record_id id);
	
	types::type_id recordTypeId() const;
	void setRecordTypeId(types::type_id id);
	
	QString name() const;
	QString origanalName() const;
	void setName(const QString &name);
	void confirmNameChanges();
	
	QString groupName() const;
	void setGroupName(const QString &groupName);
	
	QString typeName() const;
	void setTypeName(const QString &typeName);
	
	void addTypeField(types::tfield_id id, const QString &fieldName);
	
	QString field(types::rfield_id id) const;
	QString originalField(types::rfield_id id) const;
	void addField(types::rfield_id fieldId, types::tfield_id fieldTypeId, const QString &fieldData);
	
	void removeField(types::rfield_id id);
	void confirmFieldChanges(types::rfield_id id);
	
	void clear();
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
signals:
	void nameChanged(QString newName);
	void typeNameClicked();
	void groupNameClicked();
	
	void fieldClicked(types::rfield_id id);
	void fieldChanged(types::rfield_id id, QString newText);
	void fieldTypeChanged(types::rfield_id fieldId, types::tfield_id fieldTypeId);
private:
	// Slots
	void onNameChanged(QString newText);
	void onTypeNameClicked();
	void onGroupNameClicked();
	
	void onFieldClicked(types::rfield_id id);
	void onFieldChanged(types::rfield_id id, QString newText);
	void onFieldTypeChanged(types::rfield_id fieldId, int typeFieldIndex);
	
	
	// Widgets
	QVBoxLayout *mainLayout_;
	
	LineEditConfirm *nameLineEdit_;
	
	QWidget *headWidget_;
	QFormLayout *headLayout_;
	
	LabelButton *typeLabel_;
	QLabel *typeContent_;
	
	LabelButton *groupLabel_;
	QLabel *groupContent_;
	
	QFrame *hLine_;
	
	QScrollArea *scrollArea_;
	QFormLayout *formLayout_;
	
	
	// Data
	types::record_id recordId_;
	types::type_id recordTypeId_;
	
	// Field types for current type
	QMap<types::tfield_id, QString> typeFieldNames_;
	QList<types::tfield_id> typeFieldsOrder_;
	
	// Field type ComboBox + field data LineEdit
	QMap<types::rfield_id, QPair<QComboBox *, LineEditConfirm *>> fields_;
};

#endif // PASSWORDCONTENTWIDGET_H
