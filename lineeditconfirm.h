// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LINEEDITCONFIRM_H
#define LINEEDITCONFIRM_H

#include <QWidget>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

#include <QString>

class LineEditConfirm:
	public QFrame
{
	Q_OBJECT
public:
	LineEditConfirm(QWidget *parent = nullptr);
	LineEditConfirm(const QString &title, QWidget *parent = nullptr);
	
	void setText(const QString &text);
	QString text() const;
	QString originalText() const;
	
	void setFrame(bool status);
	
	// Slots
	void confirmText();
	void restoreText();
signals:
	void confirmButtonClicked(LineEditConfirm *object, QString newText);
private:
	void showButtons();
	void hideButtons();
	
	// Slots
	void onTextEdited(const QString &newText);
	void onConfirmButtonClicked();
	void onCancelButtonClicked();
	
	static QIcon confirmIcon_, cancelIcon_;
	
	QHBoxLayout *layout_;
	QLineEdit *lineEdit_;
	QPushButton *confirmButton_, *cancelButton_;
	
	QString originalText_;
};

#endif // LINEEDITCONFIRM_H
