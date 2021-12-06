#include "BiliBiliCard.h"

BiliBiliCard::BiliBiliCard(const QString& url, const QString& cid, const QString& title, int view, int danmaku, int reply, int favorite, int coin, int share, int like, int total)
	: m_url(url),
	  m_cid(cid),
	  m_title(title),
	  m_view(view),
	  m_danmaku(danmaku),
	  m_reply(reply),
	  m_favorite(favorite),
	  m_coin(coin),
	  m_share(share),
	  m_like(like),
	  m_total(total) {
	m_is_null = false;
}

QString BiliBiliCard::getUrl() const {
	return m_url;
}
QString BiliBiliCard::getCid() const {
	return m_cid;
}
QString BiliBiliCard::getTitle() const {
	return m_title;
}
int BiliBiliCard::getView() const {
	return m_view;
}
int BiliBiliCard::getDanmaku() const {
	return m_danmaku;
}
int BiliBiliCard::getReply() const {
	return m_reply;
}
int BiliBiliCard::getFavorite() const {
	return m_favorite;
}
int BiliBiliCard::getCoin() const {
	return m_coin;
}
int BiliBiliCard::getShare() const {
	return m_share;
}
int BiliBiliCard::getLike() const {
	return m_like;
}
int BiliBiliCard::getTotal() const {
	return m_total;
}
BiliBiliCard::BiliBiliCard() {
	m_is_null = true;
}

bool BiliBiliCard::is_null() const {
	return m_is_null;
}

QDebug operator<<(QDebug debug, const BiliBiliCard& messageCard) {
	debug << "m_url: " << messageCard.getUrl() << " m_cid: " << messageCard.getCid()
		  << " m_view: " << messageCard.getView() << " m_danmaku: " << messageCard.getDanmaku()
		  << " m_reply: " << messageCard.getReply() << " m_favorite: " << messageCard.getFavorite()
		  << " m_coin: " << messageCard.getCoin() << " m_share: " << messageCard.getShare()
		  << " m_like: " << messageCard.getLike() << " m_total: " << messageCard.getTotal();
	return debug;
}
