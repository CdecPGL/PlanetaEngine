#pragma once

#include <string>

#include "StringUtility.hpp"

namespace plnt {
	enum class log_level { message, warning, error, fatal };

	/*! ログマネージャ公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_log_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_log_manager() = 0 { }

		/*! レベルを指定してログを出力(レベル、発生個所、詳細(複数指定することで連結される。))*/
		template <typename... Details>
		void log(const log_level level, const std::string &place, Details &&... details) {
			log_proc(level, util::ConvertAndConnectToString(std::forward<Details>(details)...), place);
		}

		/*! シンプルなログを出力*/
		template <typename... Details>
		void simple_log(Details &&... details) {
			simple_log_proc(util::ConvertAndConnectToString(std::forward<Details>(details)...));
		}

	private:
		virtual void log_proc(log_level level, const std::string &detail, const std::string &place) = 0;
		virtual void simple_log_proc(const std::string &detail) = 0;
	};
}
