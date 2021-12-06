#include "NewTaskDialog.h"

NewTaskDialog::NewTaskDialog(QWidget *parent)
	: QDialog(parent) {
	this->setWindowTitle("添加任务");
	this->setWindowIcon(QIcon(":/Res/icon.png"));

	// Init UI
	mainLay = new QGridLayout(this);
	mainLay->addWidget(new QLabel("ID："), 0, 0, 1, 1);
	leId = new QLineEdit;
	mainLay->addWidget(leId, 0, 1, 1, 2);

	mainLay->addWidget(new QLabel("ID类型："), 1, 0, 1, 1);
	QHBoxLayout *radioLay = new QHBoxLayout;
	rdAid = new QRadioButton("aid");
	rdBvid = new QRadioButton("bvid");
	radioLay->addWidget(rdAid);
	radioLay->addWidget(rdBvid);
	rdBvid->setChecked(true);
	mainLay->addLayout(radioLay, 1, 1, 1, 2);

	btnOK = new QPushButton("确定");
	btnCancel = new QPushButton("取消");
	mainLay->addWidget(btnOK, 2, 0);
	mainLay->addWidget(btnCancel, 2, 2);

	// Connect
	connect(btnOK, &QPushButton::clicked, this, &NewTaskDialog::slotBtnOKClicked);
	connect(btnCancel, &QPushButton::clicked, this, &NewTaskDialog::close);
}

void NewTaskDialog::slotBtnOKClicked() {
	if (leId->text().isEmpty()) {
		QMessageBox::critical(this, "添加任务失败", "请先填写视频ID");
		return;
	}

	if (rdAid->isChecked())
		emit sigNewTaskAdded(0, leId->text());
	else
		emit sigNewTaskAdded(1, leId->text());
	this->reject();
}

void NewTaskDialog::closeEvent(QCloseEvent *ev) {
	// 重写退出事件
	int ret = QMessageBox::question(this, "推出确认", "确认要退出任务添加吗？");
	if (ret == QMessageBox::Yes) {
		ev->accept();
	} else
		ev->ignore();
}

NewTaskDialog::~NewTaskDialog() {
}