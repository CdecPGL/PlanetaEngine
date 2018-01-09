#pragma once

#include <string>
#include "StringUtility.h"

namespace planeta {
	enum class LogLevel { Message, Warning, Error, Fatal };
	/*! ログマネージャ公開インターフェイス*/
	class ILogManager {
	public:
		virtual ~ILogManager() = 0 {};
		/*! レベルを指定してログを出力(レベル、発生個所、詳細(複数指定することで連結される。))*/
		template<typename... Details>
		void Log(LogLevel level, const std::string& place, Details&&... details) {
			LogProc(level, util::ConvertAndConnectToString(std::forward<Details>(details)...), place);
		}
		/*! シンプルなログを出力*/
		template<typename... Details>
		void SimpleLog(Details&&... details) {
			SimpleLogProc(util::ConvertAndConnectToString(std::forward<Details>(details)...));
		}
	private:
		virtual void LogProc(LogLevel level, const std::string& detail, const std::string& place) = 0;
		virtual void SimpleLogProc(const std::string& detail) = 0;
	};
}
