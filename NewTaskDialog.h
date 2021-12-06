#ifndef UP_ASSISTANT_NEWTASKDIALOG_H
#define UP_ASSISTANT_NEWTASKDIALOG_H

#include <QCloseEvent>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>

class NewTaskDialog : public QDialog {
	Q_OBJECT
public:
	explicit NewTaskDialog(QWidget* parent = nullptr);
	~NewTaskDialog();

public slots:
	void closeEvent(QCloseEvent*);

private slots:
	void slotBtnOKClicked();

signals:
	void sigNewTaskAdded(int type, QString id);

private:
	QGridLayout* mainLay;
	QLineEdit* leId;
	QRadioButton* rdAid;
	QRadioButton* rdBvid;
	QPushButton* btnOK;
	QPushButton* btnCancel;
};


#endif//UP_ASSISTANT_NEWTASKDIALOG_H
