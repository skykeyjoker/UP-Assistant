#include "UpAssistantWidget.h"

UpAssistantWidget::UpAssistantWidget(QWidget* parent) : QMainWindow(parent) {
	this->setWindowTitle("UP主小助手");
	this->setWindowIcon(QIcon(":/Res/icon.png"));
	this->setMinimumSize(600, 400);

	// Init UI
	initUI();

	// Load Setting and Tasks
	loadSetting();

	// 初始化日志文件
	initLog();

	// 开启定时任务，定时查询
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &UpAssistantWidget::slotTimeOut);
	m_timer->start(m_timeOut * 60 * 1000);// m_timeOut以分钟为单位
										  //m_timer->start(20000);
}

// 加载设置
void UpAssistantWidget::loadSetting() {
	using Json = nlohmann::json;
	using namespace Diana;

	QFile configFile("config.json");
	if (configFile.open(QFile::ReadOnly)) {
		QByteArray configData = configFile.readAll();
		Json configDataJson = Json::parse(configData.data(), nullptr, false);
		if (!configDataJson.is_null()) {
			m_timeOut = configDataJson["timeout"].get<int>();
			qDebug() << "timeout:" << m_timeOut;
			Json tasksJsonMenu = configDataJson["tasks"];
			for (int i = 0; i < tasksJsonMenu.size(); ++i) {
				auto currentTaskJson = configDataJson["tasks"][i];
				int currentTaskType = currentTaskJson["type"].get<int>();
				qDebug() << currentTaskType;
				switch (currentTaskType) {
					case 0: {
						// aid
						QString currentTaskAid = QString::fromStdString(currentTaskJson["aid"].get<std::string>());
						QString currentTaskCid = QString::fromStdString(currentTaskJson["cid"].get<std::string>());
						BVideo* currentTask = new AidVideo(currentTaskAid, currentTaskCid);
						m_taskList.push_back(currentTask);// 插入任务列表
						qDebug() << "Aid Type Task:" << currentTaskAid << currentTaskCid;

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(currentTaskAid));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("aid"));

						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					case 1: {
						// bvid
						QString currentTaskBvid = QString::fromStdString(currentTaskJson["bvid"].get<std::string>());
						QString currentTaskCid = QString::fromStdString(currentTaskJson["cid"].get<std::string>());
						BVideo* currentTask = new BvidVideo(currentTaskBvid, currentTaskCid);
						m_taskList.push_back(currentTask);// 插入任务列表
						qDebug() << "Bvid Type Task:" << currentTaskBvid << currentTaskCid;

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(currentTaskBvid));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("bvid"));

						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					default:
						break;
				}
			}

			configFile.close();

			// 更新状态栏任务数量
			le_TaskNum->setText(tr("当前共有%1个任务").arg(m_taskList.count()));
		} else {
			QMessageBox::critical(this, "配置加载失败", "未能正确解析配置文件！");
			exit(1);
		}
	} else {
		// 文件打开失败
		QMessageBox::critical(this, "配置加载失败", "未能正确打开配置文件！请检查程序运行目录下的config.json文件！");
		exit(1);
	}
}

// 加载UI
void UpAssistantWidget::initUI() {
	mainWidget = new QWidget;
	this->setCentralWidget(mainWidget);
	mainLay = new QVBoxLayout(mainWidget);// Main Layout

	taskTable = new QTableWidget;
	mainLay->addWidget(taskTable);

	m_systemTrayIcon = new QSystemTrayIcon(this);
	m_systemTrayIcon->setIcon(QIcon(":/Res/icon.png"));
	m_systemTrayIcon->setToolTip("UP主小助手");
	m_trayMenu = new QMenu(this);
	m_actionShow = new QAction("显示", m_trayMenu);
	m_actionExit = new QAction("退出", m_trayMenu);
	m_trayMenu->addAction(m_actionShow);
	m_trayMenu->addAction(m_actionExit);
	m_systemTrayIcon->setContextMenu(m_trayMenu);
	m_systemTrayIcon->show();
	connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &UpAssistantWidget::slotTrayActivated);
	connect(m_actionShow, &QAction::triggered, [this]() {
		if (!this->isVisible()) {
			this->setWindowFlags(Qt::Window);//显示之前恢复
			showNormal();
		}
	});
	connect(m_actionExit, &QAction::triggered, this, &UpAssistantWidget::close);

	// 初始化TableWidget样式
	taskTable->setColumnCount(2);
	taskTable->setHorizontalHeaderLabels(QStringList() << "地址"
													   << "类型");
	// 让地址随拉伸变化
	taskTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	//设置table属性
	taskTable->setFocusPolicy(Qt::NoFocus);                         //失去焦点，让taskTable在未选中前不能删除
	taskTable->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行行为，以行为单位
	taskTable->setSelectionMode(QAbstractItemView::SingleSelection);//设置选择模式，选择单行
	taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置为禁止编辑

	btnLay = new QHBoxLayout;
	btnNewTask = new QPushButton("新建任务");
	btnDelTask = new QPushButton("删除任务");
	btnLay->addStretch();
	btnLay->addWidget(btnNewTask);
	btnLay->addStretch();
	btnLay->addWidget(btnDelTask);
	btnLay->addStretch();
	mainLay->addLayout(btnLay);

	// connect
	connect(btnNewTask, &QPushButton::clicked, this, &UpAssistantWidget::slotBtnNewTaskClicked);
	connect(btnDelTask, &QPushButton::clicked, this, &UpAssistantWidget::slotBtnDelTaskClicked);

	// Status Bar
	this->statusBar()->showMessage("UP主小助手启动成功！", 4000);

	le_TaskNum = new QLabel(tr("当前共有%1个任务").arg(0));
	this->statusBar()->addPermanentWidget(le_TaskNum);
}

// 初始化日志文件
void UpAssistantWidget::initLog() {
	QDir rootDir = QDir::current();
	qDebug() << rootDir;

	if (!rootDir.exists("Logs"))// 创建Logs文件
		rootDir.mkdir("Logs");

	// 创建当天日志文件
	m_logFileName = "Logs/" + QDate::currentDate().toString("yyyyMMdd") + ".xlsx";
	qDebug() << m_logFileName;

	if (!rootDir.exists(m_logFileName)) {
		// 当前日志文件不存在
		qDebug() << "创建日志文件" << m_logFileName;
		// 格式化日志文件内容
		QXlsx::Document logXlsx;

		// 写入Header
		Format header;
		header.setFontBold(true);
		header.setFontSize(12);
		header.setHorizontalAlignment(Format::AlignHCenter);
		header.setVerticalAlignment(Format::AlignVCenter);
		logXlsx.write("A1", "ID", header);
		logXlsx.write("B1", "CID", header);
		logXlsx.write("C1", "标题", header);
		logXlsx.write("D1", "点赞数", header);
		logXlsx.write("E1", "评论数", header);
		logXlsx.write("F1", "收藏数", header);
		logXlsx.write("G1", "投币数", header);
		logXlsx.write("H1", "分享数", header);
		logXlsx.write("I1", "弹幕数", header);
		logXlsx.write("J1", "播放量", header);
		logXlsx.write("K1", "正在观看", header);

		// 设置列宽行宽
		logXlsx.setRowHeight(1, 40.0);
		logXlsx.setColumnWidth(1, 13.0);
		logXlsx.setColumnWidth(2, 9.0);
		logXlsx.setColumnWidth(3, 45.0);
		logXlsx.setColumnWidth(4, 11, 9.0);

		// 保存文件
		if (logXlsx.saveAs(m_logFileName)) {
			qDebug() << "[debug] success to write xlsx file";
		} else {
			qDebug() << "[debug][error] failed to write xlsx file";
		}

		m_newLineIndex = 2;// 新行index
	} else {
		// 日志文件已存在
		qDebug() << "加载日志文件" << m_logFileName;
		QXlsx::Document logXlsx(m_logFileName);
		if (logXlsx.load()) {
			m_newLineIndex = 1;

			while (logXlsx.cellAt(m_newLineIndex, 1) != nullptr)
				m_newLineIndex++;

			qDebug() << "新行index：" << m_newLineIndex;
		} else {
			qDebug() << "日志文件打开失败";
		}
	}
}

// 定时执行查询任务
void UpAssistantWidget::slotTimeOut() {
	qDebug() << "执行1次定时任务";

	BiliBiliQuery* currentQueryTask = new BiliBiliQuery(m_taskList);
	QThread* currentQueryThread = new QThread;
	currentQueryTask->moveToThread(currentQueryThread);

	connect(currentQueryThread, &QThread::started,
			currentQueryTask, &BiliBiliQuery::startQuery);

	qRegisterMetaType<QVector<BiliBiliCard>>("QVector<BiliBiliCard>");
	qRegisterMetaType<BiliBiliCard>("BiliBiliCard");
	connect(currentQueryTask, QOverload<QVector<BiliBiliCard>>::of(&BiliBiliQuery::queryFinished),
			this, QOverload<QVector<BiliBiliCard>>::of(&UpAssistantWidget::writeLog));

	connect(currentQueryTask, QOverload<QVector<BiliBiliCard>>::of(&BiliBiliQuery::queryFinished),
			currentQueryThread, &QThread::quit);

	connect(currentQueryThread, &QThread::finished,
			currentQueryThread, &QThread::deleteLater);

	connect(currentQueryTask, QOverload<QVector<BiliBiliCard>>::of(&BiliBiliQuery::queryFinished),
			currentQueryTask, &BiliBiliQuery::deleteLater);

	currentQueryThread->start();
}

// 写入日志
void UpAssistantWidget::writeLog(QVector<BiliBiliCard> bilibiliCardList) {
	// 写日志时加锁
	QMutexLocker locker(&logMutex);

	QXlsx::Document logFile(m_logFileName);
	qDebug() << "写入1次日志";
	qDebug() << bilibiliCardList.size();

	// 写入时间信息
	logFile.write(QString(tr("A%1").arg(QString::number(m_newLineIndex))), QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
	Format header;
	header.setFontSize(12);
	header.setHorizontalAlignment(Format::AlignHCenter);
	header.setVerticalAlignment(Format::AlignVCenter);
	logFile.mergeCells(QString(tr("A%1:K%2").arg(QString::number(m_newLineIndex)).arg(QString::number(m_newLineIndex))), header);
	// 设置列宽行宽
	logFile.setColumnWidth(1, 13.0);
	logFile.setColumnWidth(2, 9.0);
	logFile.setColumnWidth(3, 45.0);
	logFile.setColumnWidth(4, 11, 9.0);
	logFile.setRowHeight(m_newLineIndex, 30.0);
	m_newLineIndex++;

	// 写入每条新日志
	for (auto currentLog : bilibiliCardList) {
		QString currentId = currentLog.getUrl();
		QString currentCid = currentLog.getCid();
		QString currentTitle = currentLog.getTitle();
		int currentLike = currentLog.getLike();
		int currentReply = currentLog.getReply();
		int currentFavourite = currentLog.getFavorite();
		int currentCoin = currentLog.getCoin();
		int currentShare = currentLog.getShare();
		int currentDanmaku = currentLog.getDanmaku();
		int currentView = currentLog.getView();
		QString currentTotal = currentLog.getTotal();

		logFile.write(QString(tr("A%1").arg(QString::number(m_newLineIndex))), currentId);
		logFile.write(QString(tr("B%1").arg(QString::number(m_newLineIndex))), currentCid);
		logFile.write(QString(tr("C%1").arg(QString::number(m_newLineIndex))), currentTitle);
		logFile.write(QString(tr("D%1").arg(QString::number(m_newLineIndex))), currentLike);
		logFile.write(QString(tr("E%1").arg(QString::number(m_newLineIndex))), currentReply);
		logFile.write(QString(tr("F%1").arg(QString::number(m_newLineIndex))), currentFavourite);
		logFile.write(QString(tr("G%1").arg(QString::number(m_newLineIndex))), currentCoin);
		logFile.write(QString(tr("H%1").arg(QString::number(m_newLineIndex))), currentShare);
		logFile.write(QString(tr("I%1").arg(QString::number(m_newLineIndex))), currentDanmaku);
		logFile.write(QString(tr("J%1").arg(QString::number(m_newLineIndex))), currentView);
		logFile.write(QString(tr("K%1").arg(QString::number(m_newLineIndex))), currentTotal);

		m_newLineIndex++;
	}

	// 保存日志文件
	if (logFile.save()) {
		qDebug() << "日志写入成功";
	} else {
		qDebug() << "日志写入失败";
	}
}

// 添加新任务槽函数
void UpAssistantWidget::slotBtnNewTaskClicked() {
	NewTaskDialog* newTaskDialog = new NewTaskDialog(this);

	connect(newTaskDialog, QOverload<int, QString>::of(&NewTaskDialog::sigNewTaskAdded),
			this, QOverload<int, QString>::of(&UpAssistantWidget::slotNewTaskAdded));

	newTaskDialog->exec();
}

void UpAssistantWidget::slotNewTaskAdded(int type, QString id) {
	// 请求Cid
	QString cid;
	BiliBiliQuery* cidQuery = new BiliBiliQuery(m_taskList, this);
	cid = cidQuery->getCid(id);
	if (cid.isEmpty()) {
		QMessageBox::critical(this, "添加任务失败", "无法查询视频cid，请检查任务id是否填写正确！");
		return;
	}
	qDebug() << "type:" << type << "id:" << id << "cid:" << cid;

	// 保存到配置文件，并添加到任务队列
	using Json = nlohmann::json;
	QFile configFile("config.json");
	if (configFile.open(QFile::ReadWrite)) {
		QByteArray configData = configFile.readAll();
		if (!configData.isEmpty()) {
			Json configDataJson = Json::parse(configData.data(), nullptr, false);
			if (!configDataJson.is_null()) {
				Json& tasksMenu = configDataJson["tasks"];
				Json currentTask;
				switch (type) {
					case 0: {
						// aid
						currentTask["type"] = 0;
						currentTask["aid"] = id.toStdString();

						// 添加到任务队列
						Diana::BVideo* currentTaskVideo = new Diana::AidVideo(id, cid);
						m_taskList.push_back(currentTaskVideo);

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(id));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("aid"));

						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					case 1: {
						// bvid
						currentTask["type"] = 1;
						currentTask["bvid"] = id.toStdString();

						// 添加到任务队列
						Diana::BVideo* currentTaskVideo = new Diana::BvidVideo(id, cid);
						m_taskList.push_back(currentTaskVideo);

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(id));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("bvid"));

						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					default:
						break;
				}
				currentTask["cid"] = cid.toStdString();
				tasksMenu.push_back(currentTask);

				// 写入到配置文件
				QByteArray writeData = QString::fromStdString(configDataJson.dump(2)).toUtf8();
				qDebug() << writeData;
				qDebug() << configFile.resize(0);// 清空文件
				configFile.write(writeData);

				le_TaskNum->setText(tr("当前共有%1个任务").arg(m_taskList.count()));

			} else {
				// 解析配置文件失败
				QMessageBox::critical(this, "添加任务失败", "无法解析配置文件，请检查运行目录下的config.json文件！");
			}
		} else {
			// 解析配置文件失败
			QMessageBox::critical(this, "添加任务失败", "无法解析配置文件，请检查运行目录下的config.json文件！");
		}
		configFile.close();
	} else {
		// 配置文件打开失败
		QMessageBox::critical(this, "添加任务失败", "无法打开配置文件，请检查运行目录下的config.json文件！");
	}
}

// 删除任务槽函数
void UpAssistantWidget::slotBtnDelTaskClicked() {
	if (taskTable->currentRow() == -1) {
		// 未选中某行任务
		QMessageBox::critical(this, "删除任务失败", "请先选中要删除的任务！");
		return;
	}
	QString targetId = taskTable->item(taskTable->currentRow(), 0)->text();
	QString targetType = taskTable->item(taskTable->currentRow(), 1)->text();
	qDebug() << "targetId" << targetId;

	// 从任务队列中删除
	for (int i = 0; i < m_taskList.size(); ++i) {
		auto currentTask = m_taskList.at(i);
		if (currentTask->getId() == targetId) {
			m_taskList.remove(i);
			break;
		}
	}

	// 从taskTable中删除
	for (int i = 0; i < taskTable->rowCount(); ++i) {
		if (taskTable->item(i, 0)->text() == targetId) {
			taskTable->removeRow(i);
			break;
		}
	}

	le_TaskNum->setText(tr("当前共有%1个任务").arg(m_taskList.count()));

	// 保存配置文件
	using Json = nlohmann::json;
	QFile configFile("config.json");
	if (configFile.open(QFile::ReadWrite)) {
		QByteArray configData = configFile.readAll();
		if (!configData.isEmpty()) {
			Json configDataJson = Json::parse(configData.data(), nullptr, false);
			if (!configDataJson.is_null()) {
				Json& taskMenu = configDataJson["tasks"];
				for (auto taskIte = taskMenu.begin(); taskIte != taskMenu.end(); ++taskIte) {
					if (taskIte->contains(targetType.toStdString())) {
						if (QString::fromStdString((*taskIte)[targetType.toStdString()].get<std::string>()) == targetId) {
							qDebug() << "Find target";
							taskMenu.erase(taskIte);
							break;
						}
					}
				}
				// 写入文件
				QByteArray writeData = QString::fromStdString(configDataJson.dump(2)).toUtf8();
				configFile.resize(0);
				configFile.write(writeData);
			} else {
				// 解析配置文件失败
				QMessageBox::critical(this, "添加任务失败", "无法解析配置文件，请检查运行目录下的config.json文件！");
			}
		} else {
			// 解析配置文件失败
			QMessageBox::critical(this, "添加任务失败", "无法解析配置文件，请检查运行目录下的config.json文件！");
		}
		configFile.close();
	} else {
		// 配置文件打开失败
		QMessageBox::critical(this, "删除任务失败", "无法打开配置文件，请检查运行目录下的config.json文件！");
	}
}

void UpAssistantWidget::slotTrayActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::Context:
			trayShowMenu();
			break;
		case QSystemTrayIcon::Trigger:
			if (!this->isVisible()) {
				this->setWindowFlags(Qt::Window);//显示之前恢复
				showNormal();
			}
			trayShowMessage();
			break;
	}
}

void UpAssistantWidget::trayShowMessage() {
	m_systemTrayIcon->showMessage("UP主小助手",
								  tr("%1个任务执行中").arg(taskTable->rowCount()),
								  QSystemTrayIcon::MessageIcon::Information,
								  2000);
}

void UpAssistantWidget::trayShowMenu() {
	m_trayMenu->show();
}

void UpAssistantWidget::changeEvent(QEvent* ev) {
	if (ev->type() == QEvent::WindowStateChange && this->isMinimized()) {
		m_systemTrayIcon->showMessage("UP主小助手", "UP主小助手已隐藏到托盘！");
		this->hide();
		ev->ignore();
	}
}

void UpAssistantWidget::closeEvent(QCloseEvent* ev) {
	int ret = QMessageBox::question(this, "请求确认", "确定要退出UP主小助手吗？");
	if (ret == QMessageBox::Yes)
		ev->accept();
	else
		ev->ignore();
}

UpAssistantWidget::~UpAssistantWidget() noexcept {
}