#include "BiliBiliQuery.h"

BiliBiliQuery* BiliBiliQuery::instance = nullptr;// static成员需要初始化

BiliBiliQuery::BiliBiliQuery() {
}

BiliBiliQuery::~BiliBiliQuery() noexcept {
	networkAccessManager.deleteLater();
}

BiliBiliQuery* BiliBiliQuery::getInstance() {
	if (instance == nullptr) {
		instance = new BiliBiliQuery;
	}
	return instance;
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
	// https://api.bilibili.com/x/web-interface/archive/stat?bvid=BV1Mq4y1g7wE
	// https://api.bilibili.com/x/player/online/total?cid=444923218&aid=549331981&bvid=BV1Mq4y1g7wE
	using Json = nlohmann::json;

	Json statJson; // stat接口
	Json totalJson;// total接口
	QString statRequestUrl;
	QString totalRequestUrl;
	QString url;
	QString cid;
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
			Diana::BvidVideo* currentVideo = dynamic_cast<Diana::BvidVideo*>(bvideo);
			url = currentVideo->getBvid();
			cid = currentVideo->getCid();
			statRequestUrl = "https://api.bilibili.com/x/web-interface/archive/stat?bvid=" + url;
			totalRequestUrl = "https://api.bilibili.com/x/player/online/total?cid=" + cid + "&bvid=" + url;
			qDebug() << url << cid << statRequestUrl << totalRequestUrl;

			break;
		}
		case Diana::AidVideoType: {
			// avid
			Diana::AidVideo* currentVideo = dynamic_cast<Diana::AidVideo*>(bvideo);
			url = currentVideo->getAid();
			cid = currentVideo->getCid();
			statRequestUrl = "https://api.bilibili.com/x/web-interface/archive/stat?aid=" + url;
			totalRequestUrl = "https://api.bilibili.com/x/player/online/total?cid=" + cid + "&aid=" + url;

			break;
		}
		default:
			break;
	}

	qDebug() << statRequestUrl;
	qDebug() << totalRequestUrl;

	// 请求stat接口
	statJson = getJson(statRequestUrl);
	if (statJson.is_null())
		return BiliBiliCard();
	// 请求total接口
	totalJson = getJson(totalRequestUrl);
	if (totalJson.is_null())
		return BiliBiliCard();

	view = statJson["data"]["view"].get<int>();
	danmaku = statJson["data"]["danmaku"].get<int>();
	reply = statJson["data"]["reply"].get<int>();
	favorite = statJson["data"]["favorite"].get<int>();
	coin = statJson["data"]["coin"].get<int>();
	share = statJson["data"]["coin"].get<int>();
	like = statJson["data"]["like"].get<int>();
	total = QString::fromStdString(totalJson["data"]["total"].get<std::string>()).toInt();

	BiliBiliCard retCard(url, cid, view, danmaku, reply, favorite, coin, share, like, total);

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
	}

	return retJson;
}
