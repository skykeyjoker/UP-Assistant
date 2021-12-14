#ifndef UP_ASSISTANT_BILIBILICARD_H
#define UP_ASSISTANT_BILIBILICARD_H

#include <QDebug>
#include <QtCore>
#include <QtGlobal>
#include <ostream>

class BiliBiliCard {
public:
	explicit BiliBiliCard(const QString &url, const QString &cid, const QString &title, int view, int danmaku, int reply, int favorite, int coin, int share, int like, const QString &total);
	explicit BiliBiliCard();
	~BiliBiliCard() = default;

public:
	bool is_null() const;
	QString getUrl() const;
	QString getCid() const;
	QString getTitle() const;
	int getView() const;
	int getDanmaku() const;
	int getReply() const;
	int getFavorite() const;
	int getCoin() const;
	int getShare() const;
	int getLike() const;
	QString getTotal() const;
	friend std::ostream &operator<<(std::ostream &os, const BiliBiliCard &card);

private:
	QString m_url;
	QString m_cid;
	QString m_title;
	int m_view;
	int m_danmaku;
	int m_reply;
	int m_favorite;
	int m_coin;
	int m_share;
	int m_like;
	QString m_total;
	bool m_is_null;
};

QDebug operator<<(QDebug debug, const BiliBiliCard &messageCard);

#endif//UP_ASSISTANT_BILIBILICARD_H
