// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef WARNINGWINDOW_H
#define WARNINGWINDOW_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>

#include <QString>

class WarningWindow: public QFrame
{
	Q_OBJECT
public:
	WarningWindow(QWidget *parent = nullptr);
	
	void setTitle(const QString &title);
	QString title() const;
	
	void setText(const QString &text);
	QString text() const;
private:
	QLabel *label_;
	QPushButton *okButton_;
	QVBoxLayout *mainLayout_;
	QHBoxLayout *buttonLayout_;
};

#endif // WARNINGWINDOW_H
