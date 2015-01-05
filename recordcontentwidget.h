// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef PASSWORDCONTENTWIDGET_H
#define PASSWORDCONTENTWIDGET_H

#include <QtWidgets>

#include <QString>
#include <QList>
#include <QPair>

#include <unordered_map>

#include "settingssaver.h"

class RecordContentWidget:
	public QGroupBox,
	public SettingsSaver
{
	Q_OBJECT
public:
	RecordContentWidget(QWidget *parent = nullptr);
	~RecordContentWidget();
	
	void setFields(const QList<QPair<QString, QString>> &fields);
	void clearFields();
	
	void readSettings(QSettings &settings, const QString &prefix = "") override;
	void writeSettings(QSettings &settings, const QString &prefix = "") const override;
private:
	QVBoxLayout *mainLayout_;
	
	QScrollArea *scrollArea_;
	QFormLayout *formLayout_;
	
	std::unordered_map<QPushButton *, QLineEdit *> buttonEditMap_;
};

#endif // PASSWORDCONTENTWIDGET_H
