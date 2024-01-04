#include "standard_config_manager.hpp"
#include "json_file.hpp"
#include "log_utility.hpp"
#include "string_utility.hpp"

namespace plnt::private_ {
	bool standard_config_manager::load_system_config(const file &file) {
		const json_file json_file{};
		//FileからJSONリソースを作成する
		if (!json_file.load(file)) {
			PE_LOG_ERROR("システム設定ファイルをJSONファイルとして読み込むことができませんでした。");
			return false;
		}
		try {
			const auto root_obj = json_file.get_root().get_with_exception<json_object>();
			//Game
			const auto game_obj = root_obj->at_with_exception("game")->get_with_exception<json_object>();
			game_title_ = *game_obj->at_with_exception("title")->get_with_exception<std::string>();
			auto buf_array = *game_obj->at_with_exception("version")->get_with_exception<std::vector<int>>();
			game_version_numbers_[0] = buf_array[0];
			game_version_numbers_[1] = buf_array[1];
			game_version_numbers_[2] = buf_array[2];
			//Engine
			const auto engine_obj = root_obj->at_with_exception("engine")->get_with_exception<json_object>();
			color_bit_depth_ = *engine_obj->at_with_exception("color_bit_depth")->get_with_exception<int>();
			buf_array = *engine_obj->at_with_exception("draw_size")->get_with_exception<std::vector<int>>();
			draw_size_.set(buf_array[0], buf_array[1]);
			is_cursor_visible_ = *engine_obj->at_with_exception("is_cursor_visible")->get_with_exception<bool>();
			//Program
			const auto program_obj = root_obj->at_with_exception("program")->get_with_exception<json_object>();
			const auto scene_obj = program_obj->at_with_exception("scene")->get_with_exception<json_object>();
			//-シーン
			startup_scene_id_ = *scene_obj->at_with_exception("startup")->get_with_exception<std::string>();
			//-衝突システム
			const auto col_obj = program_obj->at_with_exception("collision")->get_with_exception<json_object>();
			//--衝突グループ
			const std::vector<std::string> group_list = *col_obj->at_with_exception("groups")->get_with_exception<
				std::vector<std::string>>();
			collision_group_matrix_.add_collision_groups(group_list);
			//--衝突可能マトリックス
			for (auto col_mtx = *col_obj->at_with_exception("collidable_matrix")->get_with_exception<std::unordered_map<
				     std::string, std::vector<std::string>>>(); auto &[group, cbl_groups] : col_mtx) {
				for (auto &&cbl_group : cbl_groups) {
					if (!collision_group_matrix_.set_collision_flag(group, cbl_group, true)) {
						PE_LOG_WARNING("衝突グループ\"", group, "\"と\"", cbl_group,
						               "\"のフラグの設定に失敗しました。どちらかのグループが定義されていない可能性があります。");
					}
				}
			}
		} catch (std::out_of_range &e) {
			PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。内容が不足している可能性があります。(", e.what(), ")");
			return false;
		} catch (json_type_error &e) {
			PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。型が間違えている可能性があります。(", e.what(), ")");
			return false;
		}
		//設定データ取得成功
		//バージョン文字列設定
		game_version_string_ = util::convert_and_connect_to_string(game_version_numbers_[0], ".",
		                                                       game_version_numbers_[1], ".",
		                                                       game_version_numbers_[2]);
		//設定をログに出力
		auto &sys_log = *game::instance().log_manager();
		PE_LOG_MESSAGE("システム設定を読み込みました。");
		sys_log.simple_log("--------システム設定情報--------");
		sys_log.simple_log("ゲームタイトル : ", game_title_);
		sys_log.simple_log("ゲームバージョン : ", game_version_string_);
		sys_log.simple_log("カラービット深度 : ", color_bit_depth_);
		sys_log.simple_log("描画(内部)サイズ : ", draw_size_.to_string());
		sys_log.simple_log("--------------------------------");
		return true;
	}

	bool standard_config_manager::load_user_config(const file &file) {
		const json_file json_file{};
		//FileからJSONリソースを作成する
		if (!json_file.load(file)) {
			PE_LOG_ERROR("ユーザー設定ファイルをJSONファイルとして読み込むことができませんでした。");
			return false;
		}
		try {
			const auto root_obj = json_file.get_root().get_with_exception<json_object>();
			//Window
			const auto window_obj = root_obj->at_with_exception("window")->get_with_exception<json_object>();
			const auto buf_array = *window_obj->at_with_exception("window_size")->get_with_exception<std::vector<int>>();
			window_size_.set(buf_array[0], buf_array[1]);
			is_window_mode_ = *window_obj->at_with_exception("window_mode")->get_with_exception<bool>();
		} catch (std::out_of_range &e) {
			PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。内容が不足している可能性があります。(", e.what(), ")");
			return false;
		} catch (json_type_error &e) {
			PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。型が間違えている可能性があります。(", e.what(), ")");
			return false;
		}
		//設定データ取得成功
		//設定をログに出力
		auto &sys_log = *game::instance().log_manager();
		PE_LOG_MESSAGE("ユーザー設定を読み込みました。");
		sys_log.simple_log("--------ユーザー設定情報--------");
		sys_log.simple_log("ウインドウモード : ", is_window_mode_ ? "有効" : "無効");
		sys_log.simple_log("ウインドウ(表示)サイズ : ", window_size_.to_string());
		sys_log.simple_log("--------------------------------");
		return true;
	}

	int standard_config_manager::color_bit_depth() const { return color_bit_depth_; }

	std::string standard_config_manager::startup_scene_id() const { return startup_scene_id_; }

	std::string standard_config_manager::game_title() const { return game_title_; }

	std::string standard_config_manager::game_version_string() const { return game_version_string_; }

	std::array<int, 3> standard_config_manager::game_version_numbers() const { return game_version_numbers_; }

	bool standard_config_manager::is_window_mode() const { return is_window_mode_; }

	vector_2di standard_config_manager::draw_size() const { return draw_size_; }

	vector_2di standard_config_manager::window_size() const { return window_size_; }

	const collision_group_matrix &standard_config_manager::collision_group_matrix() const {
		return collision_group_matrix_;
	}

	bool standard_config_manager::is_cursor_visible() const { return is_cursor_visible_; }
}
