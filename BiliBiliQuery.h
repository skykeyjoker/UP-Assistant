#ifndef UP_ASSISTANT_BILIBILIQUERY_H
#define UP_ASSISTANT_BILIBILIQUERY_H

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <memory>

#include "BVideo.h"
#include "BiliBiliCard.h"
#include "json.hpp"

class BiliBiliQuery : public QObject {
	Q_OBJECT

public:
	static BiliBiliQuery* getInstance();

private:
	BiliBiliQuery();
	~BiliBiliQuery();
	BiliBiliQuery(const BiliBiliQuery&) = default;
	BiliBiliQuery& operator=(const BiliBiliQuery&) = default;

public:
	QString getCid(const QString& url);
	BiliBiliCard getBiliBiliCard(Diana::BVideo* bvideo);

private:
	nlohmann::json getJson(const QString& url);

private:
	static BiliBiliQuery* instance;

private:
	QNetworkAccessManager networkAccessManager;
};


#endif//UP_ASSISTANT_BILIBILIQUERY_H
