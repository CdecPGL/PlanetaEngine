#pragma once

#include <string>
#include <vector>

#include "weak_pointer.hpp"

namespace plnt {
	class i_game_object;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_game_object_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_game_object_manager() = 0 {}
		/*定義ファイルを指定してゲームオブジェクトを作成*/
		[[nodiscard]] virtual weak_pointer<i_game_object> create_game_object(const std::string &game_object_def_file_id) = 0;
		/*定義ファイルを指定して名前付きゲームオブジェクトを作成*/
		[[nodiscard]] virtual weak_pointer<i_game_object> create_game_object(const std::string &game_object_def_file_id,
		                                                    const std::string &name) = 0;
		/*コンポーネントタイプIDを指定してゲームオブジェクトを作成*/
		[[nodiscard]] virtual weak_pointer<i_game_object> create_game_object_with_component_type_id_list(
			const std::vector<std::string> &game_object_component_type_id_list) = 0;
		/*コンポーネントタイプIDを指定して名前付きゲームオブジェクトを作成*/
		[[nodiscard]] virtual weak_pointer<i_game_object> create_game_object_with_component_type_id_list(
			const std::vector<std::string> &game_object_component_type_id_list, const std::string &name) = 0;
	};
}
