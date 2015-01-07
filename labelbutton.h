// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QLabel>
#include <QMouseEvent>

#include <QString>

class LabelButton: public QLabel
{
	Q_OBJECT
public:
	LabelButton(QWidget *parent = nullptr);
	LabelButton(const QString &text, QWidget *parent = nullptr);
	
	void mouseReleaseEvent(QMouseEvent *ev);
signals:
	void clicked(LabelButton *labelButton);
};

#endif // LABELBUTTON_H
