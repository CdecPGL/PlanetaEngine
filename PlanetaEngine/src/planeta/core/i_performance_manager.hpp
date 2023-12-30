#pragma once

#include "Time.hpp"

namespace plnt {
	/*! パフォーマンスマネージャの公開インターフェイス。パフォーマンスの計測や調整を行う。*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_performance_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_performance_manager() = 0 { }

		/*! 起動からの経過フレームを取得する*/
		[[nodiscard]] virtual size_t get_current_frame_count() const = 0;
		/*! 起動からの経過時間を取得する*/
		[[nodiscard]] virtual const util::time get_current_time_count() const = 0;
	};
}
