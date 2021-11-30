#include "BVideo.h"

namespace Diana {
	BVideo::~BVideo() noexcept {}// 纯虚析构函数也必须有定义体

	AidVideo::AidVideo(const QString &aid, const QString &cid) : m_aid(aid),
																 m_cid(cid) {
	}
	AidVideo::~AidVideo() noexcept {
	}
	BVideoType AidVideo::getType() const {
		return BVideoType::AidVideoType;
	}
	QString AidVideo::getAid() const {
		return m_aid;
	}
	QString AidVideo::getCid() const {
		return m_cid;
	}


	BvidVideo::BvidVideo(const QString &bvid, const QString &cid) : m_bvid(bvid),
																	m_cid(cid) {
	}
	BvidVideo::~BvidVideo() noexcept {
	}
	BVideoType BvidVideo::getType() const {
		return BVideoType::BvidVideoType;
	}
	QString BvidVideo::getBvid() const {
		return m_bvid;
	}
	QString BvidVideo::getCid() const {
		return m_cid;
	}
}// namespace Diana
