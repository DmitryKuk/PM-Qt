// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QtWidgets>

#include <QString>
#include <QList>
#include <QPair>

#include <unordered_map>

#include "hline.h"
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
	void setName(const QString &name);
	
	QString group() const;
	void setGroup(const QString &groupName);
	
	QString type() const;
	void setType(const QString &typeName);
	
	QString field(int index) const;
	QString originalField(int index) const;
	void setFields(const QList<QPair<QString, QString>> &fields);
	
	void clear();
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
signals:
	void nameClicked();
	void nameChanged(QString newName);
	
	void typeClicked();
	void typeChanged(QString newTypeName);
	
	void groupClicked();
	void groupChanged(QString newGroupName);
	
	void fieldClicked(int index);
	void fieldChanged(int index, QString newText);
private:
	// Slots
	void onNameClicked(LabelButton *button);
	void onNameChanged(QString newText);
	
	void onTypeClicked(LabelButton *button);
	void onTypeChanged(QString newText);
	
	void onGroupClicked(LabelButton *button);
	void onGroupChanged(QString newText);
	
	void onFieldClicked(LabelButton *button);
	void onFieldChanged(LineEditConfirm *field, QString newText);
	
	
	QVBoxLayout *mainLayout_;
	
	LabelButton *nameLabel_;
	
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
