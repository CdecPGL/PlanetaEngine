#pragma once

#include <string>
#include "planeta/core/NonCopyable.hpp"

namespace plnt {
	class IDebugInformationAdder;
	namespace private_ {
		class SubSystemManager : private util::NonCopyable<SubSystemManager>{
		public:
			virtual ~SubSystemManager() = 0 {};
			/*デバッグ情報追加ハンドラ*/
			virtual void DebugInfotmationAddHandler(IDebugInformationAdder& di_adder) {};
			/*デバッグ情報チャンネルを作成する*/
			void CreateDebugInformationChannel(const std::string& channel_id);
			/*デバッグ情報チャンネルを削除する*/
			void DeleteDebugInformationChannel();
		private:
			std::string debug_info_channel_id_;
		};
	}
}
