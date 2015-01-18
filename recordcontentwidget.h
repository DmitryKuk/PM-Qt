// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QString>
#include <QList>
#include <QPair>

#include <unordered_map>

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
	
	QString name() const;
	QString origanalName() const;
	void setName(const QString &name);
	void confirmNameChanges();
	
	QString groupName() const;
	void setGroupName(const QString &groupName);
	
	QString typeName() const;
	void setTypeName(const QString &typeName);
	
	QString field(int index) const;
	QString originalField(int index) const;
	void setFields(const QList<QPair<QString, QString>> &fields);
	
	void removeField(int index);
	void confirmFieldChanges(int index);
	
	void clear();
	
	virtual void readSettings(QSettings &settings, const QString &prefix = "") override;
	virtual void writeSettings(QSettings &settings, const QString &prefix = "") const override;
signals:
	void nameChanged(QString newName);
	void typeNameClicked();
	void groupNameClicked();
	
	void fieldClicked(int index);
	void fieldChanged(int index, QString newText);
private:
	// Slots
	void onNameChanged(QString newText);
	void onTypeNameClicked();
	void onGroupNameClicked();
	
	void onFieldClicked(int index);
	void onFieldChanged(int index, QString newText);
	
	
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
	
	// Label button + LineEdit + original content
	QList<QPair<LabelButton *, LineEditConfirm *>> fields_;
};

#endif // PASSWORDCONTENTWIDGET_H
