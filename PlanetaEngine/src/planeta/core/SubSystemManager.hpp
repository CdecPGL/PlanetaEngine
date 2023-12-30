#pragma once

#include <string>
#include "non_copyable.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		class SubSystemManager : private util::non_copyable<SubSystemManager> {
		public:
			virtual ~SubSystemManager() = 0 { };
			/*デバッグ情報追加ハンドラ*/
			virtual void DebugInfotmationAddHandler(i_debug_information_adder &di_adder) { };
			/*デバッグ情報チャンネルを作成する*/
			void CreateDebugInformationChannel(const std::string &channel_id);
			/*デバッグ情報チャンネルを削除する*/
			void DeleteDebugInformationChannel();

		private:
			std::string debug_info_channel_id_;
		};
	}
}
