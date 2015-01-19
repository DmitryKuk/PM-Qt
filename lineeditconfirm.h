// Author: Dmitry Kukovinets (d1021976@gmail.com)

#ifndef LINEEDITCONFIRM_H
#define LINEEDITCONFIRM_H

#include <QFrame>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPushButton>

#include <QString>

// TODO: Icons on buttons without text

class LineEditConfirm:
	public QFrame
{
	Q_OBJECT
public:
	LineEditConfirm(QWidget *parent = nullptr, bool isTitle = false);
	LineEditConfirm(const QString &title, QWidget *parent = nullptr, bool isTitle = false);
	
	void setText(const QString &text);
	QString text() const;
	QString originalText() const;
	
	void setFrame(bool status);
	
	// Slots
	void confirmText();
	void restoreText();
signals:
	void accepted(QString newText);
	void rejected();
private:
	void init(bool isTitle);
	
	void showButtons();
	void hideButtons();
	
	void onClicked(QAbstractButton *button);
	
	// Slots
	void onTextEdited(const QString &newText);
	
	QHBoxLayout *mainLayout_;
	QLineEdit *lineEdit_;
	QDialogButtonBox *buttonBox_;
	
	QString originalText_;
};

#endif // LINEEDITCONFIRM_H
