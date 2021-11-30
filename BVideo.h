#ifndef UP_ASSISTANT_BVIDEO_H
#define UP_ASSISTANT_BVIDEO_H

#include <QtCore>
#include <QtGlobal>

namespace Diana {

	enum BVideoType {
		AidVideoType,
		BvidVideoType
	};

	class BVideo {
	public:
		explicit BVideo() = default;
		~BVideo() noexcept;

	public:
		virtual BVideoType getType() const = 0;
	};

	class AidVideo : public BVideo {
	public:
		explicit AidVideo(const QString &aid, const QString &cid);
		~AidVideo() noexcept;

	public:
		BVideoType getType() const;
		QString getAid() const;
		QString getCid() const;

	private:
		QString m_aid;
		QString m_cid;
	};

	class BvidVideo : public BVideo {
	public:
		explicit BvidVideo(const QString &bvid, const QString &cid);
		~BvidVideo() noexcept;

	public:
		BVideoType getType() const;
		QString getBvid() const;
		QString getCid() const;

	private:
		QString m_bvid;
		QString m_cid;
	};

};// namespace Diana

#endif//UP_ASSISTANT_BVIDEO_H
