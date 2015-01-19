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
	
	record_id_t recordId() const;
	void setRecordId(record_id_t id);
	
	type_id_t recordTypeId() const;
	void setRecordTypeId(type_id_t id);
	
	QString name() const;
	QString origanalName() const;
	void setName(const QString &name);
	void confirmNameChanges();
	
	QString groupName() const;
	void setGroupName(const QString &groupName);
	
	QString typeName() const;
	void setTypeName(const QString &typeName);
	
	void addTypeField(tfield_id_t id, const QString &fieldName);
	
	QString field(rfield_id_t id) const;
	QString originalField(rfield_id_t id) const;
	void addField(rfield_id_t fieldId, tfield_id_t fieldTypeId, const QString &fieldData);
	
	void removeField(rfield_id_t id);
	void confirmFieldChanges(rfield_id_t id);
	
	void clear();
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
signals:
	void nameChanged(QString newName);
	void typeNameClicked();
	void groupNameClicked();
	
	void fieldClicked(rfield_id_t id);
	void fieldChanged(rfield_id_t id, QString newText);
	void fieldTypeChanged(rfield_id_t fieldId, tfield_id_t fieldTypeId);
private:
	// Slots
	void onNameChanged(QString newText);
	void onTypeNameClicked();
	void onGroupNameClicked();
	
	void onFieldClicked(rfield_id_t id);
	void onFieldChanged(rfield_id_t id, QString newText);
	void onFieldTypeChanged(rfield_id_t fieldId, tfield_id_t typeFieldIndex);
	
	
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
	record_id_t recordId_;
	type_id_t recordTypeId_;
	
	// Field types for current type
	QMap<tfield_id_t, QString> typeFieldNames_;
	QList<tfield_id_t> typeFieldsOrder_;
	
	// Field type ComboBox + field data LineEdit
	QMap<rfield_id_t, QPair<QComboBox *, LineEditConfirm *>> fields_;
};

#endif // PASSWORDCONTENTWIDGET_H
