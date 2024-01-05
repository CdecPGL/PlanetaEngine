#pragma once

#include <string>

#include "non_copyable.hpp"

namespace plnt {
	class i_debug_information_adder;

	namespace private_ {
		// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		class sub_system_manager : util::non_copyable<sub_system_manager> {
		public:
			sub_system_manager() = default;
			sub_system_manager(sub_system_manager &&) = delete;
			virtual ~sub_system_manager() = default;
			sub_system_manager &operator=(sub_system_manager &&) = delete;

			/*デバッグ情報追加ハンドラ*/
			virtual void debug_information_add_handler([[maybe_unused]] i_debug_information_adder &di_adder) {}
			/*デバッグ情報チャンネルを作成する*/
			void create_debug_information_channel(const std::string &channel_id);
			/*デバッグ情報チャンネルを削除する*/
			void delete_debug_information_channel();

		private:
			std::string debug_info_channel_id_;
		};
	}
}
