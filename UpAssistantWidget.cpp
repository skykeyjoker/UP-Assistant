#include "UpAssistantWidget.h"

UpAssistantWidget::UpAssistantWidget(QWidget* parent) : QMainWindow(parent) {
	this->setWindowTitle("UP主小助手");
	this->setMinimumSize(600, 400);

	// Init UI
	initUI();

	// Load Setting and Tasks
	loadSetting();

	// TODO 开启定时任务，定时查询
	m_query = BiliBiliQuery::getInstance();
	QString ret = m_query->getCid("BV1Mq4y1g7wE");
	qDebug() << ret;
	BiliBiliCard retCard1 = m_query->getBiliBiliCard(new Diana::AidVideo("549331981", "444923218"));
	BiliBiliCard retCard2 = m_query->getBiliBiliCard(new Diana::BvidVideo("BV1Mq4y1g7wE", "444923218"));
	qDebug() << retCard1;
	qDebug() << retCard2;
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
						m_taskList.push_back(currentTask);
						qDebug() << "Aid Type Task:" << currentTaskAid << currentTaskCid;

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(currentTaskAid));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("aid"));
						taskTable->setItem(currentRow, 2, new QTableWidgetItem("执行中"));
						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
						taskTable->item(currentRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					case 1: {
						// bvid
						QString currentTaskBvid = QString::fromStdString(currentTaskJson["bvid"].get<std::string>());
						QString currentTaskCid = QString::fromStdString(currentTaskJson["cid"].get<std::string>());
						BVideo* currentTask = new BvidVideo(currentTaskBvid, currentTaskCid);
						m_taskList.push_back(currentTask);
						qDebug() << "Bvid Type Task:" << currentTaskBvid << currentTaskCid;

						// 加载任务到TableWidget
						taskTable->insertRow(taskTable->rowCount());
						int currentRow = taskTable->rowCount() - 1;
						taskTable->setItem(currentRow, 0, new QTableWidgetItem(currentTaskBvid));// 插入地址
						taskTable->setItem(currentRow, 1, new QTableWidgetItem("bvid"));
						taskTable->setItem(currentRow, 2, new QTableWidgetItem("执行中"));
						taskTable->item(currentRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);// 居中显示
						taskTable->item(currentRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
						taskTable->item(currentRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

						break;
					}
					default:
						break;
				}
			}

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

void UpAssistantWidget::initUI() {
	mainWidget = new QWidget;
	this->setCentralWidget(mainWidget);
	mainLay = new QVBoxLayout(mainWidget);// Main Layout

	taskTable = new QTableWidget;
	mainLay->addWidget(taskTable);

	// 初始化TableWidget样式
	taskTable->setColumnCount(3);
	taskTable->setHorizontalHeaderLabels(QStringList() << "地址"
													   << "类型"
													   << "状态");
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
	btnPauseTask = new QPushButton("暂停任务");
	btnRestartTask = new QPushButton("开始任务");
	btnLay->addStretch();
	btnLay->addWidget(btnNewTask);
	btnLay->addStretch();
	btnLay->addWidget(btnDelTask);
	btnLay->addStretch();
	btnLay->addWidget(btnPauseTask);
	btnLay->addStretch();
	btnLay->addWidget(btnRestartTask);
	btnLay->addStretch();
	mainLay->addLayout(btnLay);

	// connect

	// Status Bar
	this->statusBar()->showMessage("UP主小助手启动成功！", 4000);

	le_TaskNum = new QLabel(tr("当前共有%1个任务").arg(0));
	this->statusBar()->addPermanentWidget(le_TaskNum);
}

UpAssistantWidget::~UpAssistantWidget() noexcept {
}