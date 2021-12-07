#ifndef UP_ASSISTANT_BILIBILIQUERY_H
#define UP_ASSISTANT_BILIBILIQUERY_H

#include <QDebug>
#include <QEventLoop>
#include <QMetaType>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "BVideo.h"
#include "BiliBiliCard.h"
#include "json.hpp"

class BiliBiliQuery : public QObject {
	Q_OBJECT

public:
	explicit BiliBiliQuery(const QVector<Diana::BVideo*>& taskList, QObject* parent = nullptr);
	BiliBiliQuery::~BiliBiliQuery() noexcept;

public:
	QString getCid(const QString& url);

signals:
	void queryFinished(QVector<BiliBiliCard> bilibiliCardList);

public slots:
	void startQuery();

private:
	BiliBiliCard getBiliBiliCard(Diana::BVideo* bvideo);

	nlohmann::json getJson(const QString& url);

private:
	const QVector<Diana::BVideo*>& m_taskList;// 任务列表
	QVector<BiliBiliCard> m_bilibiliCardList; // 视频信息列表

	QNetworkAccessManager networkAccessManager;
};


#endif//UP_ASSISTANT_BILIBILIQUERY_H
