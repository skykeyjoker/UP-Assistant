#ifndef UP_ASSISTANT_UPASSISTANTWIDGET_H
#define UP_ASSISTANT_UPASSISTANTWIDGET_H

#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QVBoxLayout>

#include "BVideo.h"
#include "BiliBiliCard.h"
#include "BiliBiliQuery.h"
#include "json.hpp"

class UpAssistantWidget : public QMainWindow {
	Q_OBJECT
public:
	explicit UpAssistantWidget(QWidget* parent = nullptr);
	~UpAssistantWidget() noexcept;

private:
	void loadSetting();// 加载配置文件
	void initUI();     // 加载UI

private:
	QWidget* mainWidget;
	QVBoxLayout* mainLay;

	QTableWidget* taskTable;

	QHBoxLayout* btnLay;
	QPushButton* btnNewTask;
	QPushButton* btnDelTask;
	QPushButton* btnPauseTask;
	QPushButton* btnRestartTask;

	QLabel* le_TaskNum;

private:
	BiliBiliQuery* m_query;
	int m_timeOut;
	QVector<Diana::BVideo*> m_taskList;// 任务列表
};


#endif//UP_ASSISTANT_UPASSISTANTWIDGET_H
