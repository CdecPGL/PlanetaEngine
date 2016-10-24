#pragma once

#include "PETime.h"

namespace planeta {
	/*! パフォーマンスマネージャの公開インターフェイス。パフォーマンスの計測や調整を行う。*/
	class IPerformanceManager {
	public:
		virtual ~IPerformanceManager() = 0 {};
		/*! 起動からの経過フレームを取得する*/
		virtual size_t GetCurrentFrameCount()const = 0;
		/*! 起動からの経過時間を取得する*/
		virtual const util::Time GetCurrentTimeCount()const = 0;
	};
}