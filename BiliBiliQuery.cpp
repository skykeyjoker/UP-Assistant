#include "BiliBiliQuery.h"

BiliBiliQuery::BiliBiliQuery(const QVector<Diana::BVideo*>& taskList, QObject* parent) : m_taskList(taskList),
																						 QObject(parent) {
}

BiliBiliQuery::~BiliBiliQuery() noexcept {
	networkAccessManager.deleteLater();
}

void BiliBiliQuery::startQuery() {
	// 开始查询槽函数
	for (auto currentTaskItem : m_taskList) {
		switch (currentTaskItem->getType()) {
			case Diana::AidVideoType: {
				// aid task
				Diana::AidVideo* currentTask = dynamic_cast<Diana::AidVideo*>(currentTaskItem);
				auto currentBiliBiliCard = getBiliBiliCard(currentTask);

				if (!currentBiliBiliCard.is_null()) {
					qDebug() << currentBiliBiliCard;
					m_bilibiliCardList.push_back(currentBiliBiliCard);
				} else {
					// 空的卡片，不做任何操作
				}

				break;
			}

			case Diana::BvidVideoType: {
				// bvid task
				Diana::BvidVideo* currentTask = dynamic_cast<Diana::BvidVideo*>(currentTaskItem);
				auto currentBiliBiliCard = getBiliBiliCard(currentTask);

				if (!currentBiliBiliCard.is_null()) {
					qDebug() << currentBiliBiliCard;
					m_bilibiliCardList.push_back(currentBiliBiliCard);
				} else {
					// 空的卡片，不做任何操作
				}

				break;
			}

			default:
				break;
		}
		QThread::sleep(5);// 休息5s后进行下一个任务
	}

	// 发送查询结束消息
	qRegisterMetaType<QVector<BiliBiliCard>>("QVector<BiliBiliCard>");
	qRegisterMetaType<BiliBiliCard>("BiliBiliCard");
	emit queryFinished(m_bilibiliCardList);
}

QString BiliBiliQuery::getCid(const QString& url) {
	// 通过接口获取cid
	// https://api.bilibili.com/x/player/pagelist?bvid=BV1Mq4y1g7wE&jsonp=jsonp
	using Json = nlohmann::json;

	QString currentRequestUrl = "https://api.bilibili.com/x/player/pagelist?jsonp=jsonp&";
	if (url.startsWith("BV")) {
		// bvid
		currentRequestUrl += "bvid=" + url;
	} else {
		// aid
		currentRequestUrl += "aid=" + url;
	}
	qDebug() << currentRequestUrl;
	Json cidJson = getJson(currentRequestUrl);

	if (cidJson.is_null())
		return QString();

	QString cid = QString::number(cidJson["data"][0]["cid"].get<int>());

	return cid;// 返回对应的cid
}

BiliBiliCard BiliBiliQuery::getBiliBiliCard(Diana::BVideo* bvideo) {
	// 通过接口获取当前视频信息
	// https://api.bilibili.com/x/web-interface/view?bvid=BV1VL411M7N6
	// https://api.bilibili.com/x/player/online/total?cid=444923218&aid=549331981&bvid=BV1Mq4y1g7wE
	using Json = nlohmann::json;

	Json viewJson; // stat接口
	Json totalJson;// total接口
	QString viewRequestUrl;
	QString totalRequestUrl;
	QString url;
	QString cid;
	QString title;
	int view;
	int danmaku;
	int reply;
	int favorite;
	int coin;
	int share;
	int like;
	int total;

	switch (bvideo->getType()) {
		case Diana::BvidVideoType: {
			// bvid
			//Diana::BvidVideo* currentVideo = dynamic_cast<Diana::BvidVideo*>(bvideo);
			url = bvideo->getId();
			cid = bvideo->getCid();
			viewRequestUrl = "https://api.bilibili.com/x/web-interface/view?bvid=" + url;
			totalRequestUrl = "https://api.bilibili.com/x/player/online/total?cid=" + cid + "&bvid=" + url;
			qDebug() << url << cid << viewRequestUrl << totalRequestUrl;

			break;
		}
		case Diana::AidVideoType: {
			// avid
			//Diana::AidVideo* currentVideo = dynamic_cast<Diana::AidVideo*>(bvideo);
			url = bvideo->getId();
			cid = bvideo->getCid();
			viewRequestUrl = "https://api.bilibili.com/x/web-interface/view?aid=" + url;
			totalRequestUrl = "https://api.bilibili.com/x/player/online/total?cid=" + cid + "&aid=" + url;
			qDebug() << url << cid << viewRequestUrl << totalRequestUrl;

			break;
		}
		default:
			break;
	}

	// 请求view接口
	viewJson = getJson(viewRequestUrl);
	if (viewJson.is_null())
		return BiliBiliCard();
	// 请求total接口
	totalJson = getJson(totalRequestUrl);
	if (totalJson.is_null())
		return BiliBiliCard();

	title = QString::fromStdString(viewJson["data"]["title"].get<std::string>());
	view = viewJson["data"]["stat"]["view"].get<int>();
	danmaku = viewJson["data"]["stat"]["danmaku"].get<int>();
	reply = viewJson["data"]["stat"]["reply"].get<int>();
	favorite = viewJson["data"]["stat"]["favorite"].get<int>();
	coin = viewJson["data"]["stat"]["coin"].get<int>();
	share = viewJson["data"]["stat"]["share"].get<int>();
	like = viewJson["data"]["stat"]["like"].get<int>();
	total = QString::fromStdString(totalJson["data"]["total"].get<std::string>()).toInt();

	BiliBiliCard retCard(url, cid, title, view, danmaku, reply, favorite, coin, share, like, total);

	return retCard;
}

nlohmann::json BiliBiliQuery::getJson(const QString& url) {
	using Json = nlohmann::json;
	Json retJson;

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QEventLoop eventLoop;
	QObject::connect(&networkAccessManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
	QNetworkReply* reply = networkAccessManager.get(request);
	eventLoop.exec();

	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "请求失败" << url << reply->error();
		return retJson;
	}

	QByteArray retData = reply->readAll();
	if (retData.isNull()) {
		qDebug() << "请求返回数据为空" << url;
		return retJson;
	}
	retJson = Json::parse(retData.data(), nullptr, false);
	if (retJson.is_null()) {
		qDebug() << "解析返回数据失败" << url;
		return retJson;
	}
	int code = retJson["code"].get<int>();
	if (code != 0) {
		qDebug() << "解析返回数据失败，状态码不为0" << url << code;
		qDebug() << QString::fromStdString(retJson.dump());

		return Json();
	}

	return retJson;
}
