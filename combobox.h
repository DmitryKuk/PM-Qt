// Author: Dmitry Kukovinets (d1021976@gmail.com)

// This file contains a proxy class because of QComboBox doesn't want
// to connect to lambda.

#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

class ComboBox: public QComboBox
{
	Q_OBJECT
public:
	ComboBox(QWidget *parent = nullptr);
signals:
	void changed(int newIndex);
private:
	void onCurrentIndexChanged(int newIndex);
};

#endif // COMBOBOX_H
