#ifndef UP_ASSISTANT_UPASSISTANTWIDGET_H
#define UP_ASSISTANT_UPASSISTANTWIDGET_H

#include <QAction>
#include <QCloseEvent>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHideEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <QPushButton>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include "BVideo.h"
#include "BiliBiliCard.h"
#include "BiliBiliQuery.h"
#include "NewTaskDialog.h"
#include "json.hpp"

// QXlsx
#include "3rd/QXlsx/xlsxcellrange.h"
#include "3rd/QXlsx/xlsxdocument.h"
using namespace QXlsx;

class UpAssistantWidget : public QMainWindow {
	Q_OBJECT
public:
	explicit UpAssistantWidget(QWidget* parent = nullptr);
	~UpAssistantWidget() noexcept;

public slots:
	void closeEvent(QCloseEvent*);
	void changeEvent(QEvent*);

private:
	void loadSetting();// 加载配置文件
	void initUI();     // 加载UI
	void initLog();    // 初始化Xlsx

	void trayShowMessage();
	void trayShowMenu();

private slots:
	void slotTimeOut();                                   // 定时查询
	void writeLog(QVector<BiliBiliCard> bilibiliCardList);// 写入Xlsx

	void slotBtnNewTaskClicked();
	void slotBtnDelTaskClicked();

	void slotNewTaskAdded(int type, QString id);

	void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
	QWidget* mainWidget;
	QVBoxLayout* mainLay;

	QTableWidget* taskTable;

	QHBoxLayout* btnLay;
	QPushButton* btnNewTask;
	QPushButton* btnDelTask;

	QLabel* le_TaskNum;

	QSystemTrayIcon* m_systemTrayIcon;// 托盘图标
	QMenu* m_trayMenu;
	QAction* m_actionShow;
	QAction* m_actionExit;

private:
	QTimer* m_timer;
	int m_timeOut;
	QVector<Diana::BVideo*> m_taskList;// 任务列表

	QString m_logFileName;
	int m_newLineIndex;
	QMutex logMutex;// 日志文件写入锁
};


#endif//UP_ASSISTANT_UPASSISTANTWIDGET_H
