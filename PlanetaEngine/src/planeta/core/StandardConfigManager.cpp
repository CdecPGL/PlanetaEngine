﻿#include "StandardConfigManager.hpp"
#include "json_file.hpp"
#include "LogUtility.hpp"
#include "StringUtility.hpp"

namespace plnt {
	namespace private_ {
		bool StandardConfigManager::load_system_config(const file &file) {
			json_file json_file{};
			//FileからJSONリソースを作成する
			if (!json_file.load(file)) {
				PE_LOG_ERROR("システム設定ファイルをJSONファイルとして読み込むことができませんでした。");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.get_root().get_with_exception<json_object>();
				//Game
				auto game_obj = root_obj->at_with_exception("Game")->get_with_exception<json_object>();
				game_title_ = *game_obj->at_with_exception("Title")->get_with_exception<std::string>();
				auto buf_array = *game_obj->at_with_exception("Version")->get_with_exception<std::vector<int>>();
				game_version_numbers_[0] = buf_array[0];
				game_version_numbers_[1] = buf_array[1];
				game_version_numbers_[2] = buf_array[2];
				//Engine
				auto engine_obj = root_obj->at_with_exception("Engine")->get_with_exception<json_object>();
				color_bit_depth_ = *engine_obj->at_with_exception("ColorBitDepth")->get_with_exception<int>();
				buf_array = *engine_obj->at_with_exception("DrawSize")->get_with_exception<std::vector<int>>();
				draw_size_.Set(buf_array[0], buf_array[1]);
				is_cursor_visible_ = *engine_obj->at_with_exception("IsCursorVisible")->get_with_exception<bool>();
				//Program
				auto prog_obj = root_obj->at_with_exception("Program")->get_with_exception<json_object>();
				auto scene_obj = prog_obj->at_with_exception("Scene")->get_with_exception<json_object>();
				//-シーン
				startup_scene_id_ = *scene_obj->at_with_exception("Startup")->get_with_exception<std::string>();
				//-衝突システム
				auto col_obj = prog_obj->at_with_exception("Collision")->get_with_exception<json_object>();
				//--衝突グループ
				std::vector<std::string> group_list;
				group_list = *col_obj->at_with_exception("Groups")->get_with_exception<std::vector<std::string>>();
				collision_group_matrix_.add_collision_groups(group_list);
				//--衝突可能マトリックス
				auto col_mtx = *col_obj->at_with_exception("CollidableMatrix")->get_with_exception<std::unordered_map<
					std::string, std::vector<std::string>>>();
				for (auto &&group : col_mtx) {
					for (auto &&cbl_group : group.second) {
						if (!collision_group_matrix_.set_collision_flag(group.first, cbl_group, true)) {
							PE_LOG_WARNING("衝突グループ\"", group.first, "\"と\"", cbl_group,
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
			game_version_string_ = util::ConvertAndConnectToString(game_version_numbers_[0], ".",
			                                                       game_version_numbers_[1], ".",
			                                                       game_version_numbers_[2]);
			//設定をログに出力
			auto &sys_log = *game::instance().log_manager();
			PE_LOG_MESSAGE("システム設定を読み込みました。");
			sys_log.simple_log("--------システム設定情報--------");
			sys_log.simple_log("ゲームタイトル : ", game_title_);
			sys_log.simple_log("ゲームバージョン : ", game_version_string_);
			sys_log.simple_log("カラービット深度 : ", color_bit_depth_);
			sys_log.simple_log("描画(内部)サイズ : ", draw_size_.ToString());
			sys_log.simple_log("--------------------------------");
			return true;
		}

		bool StandardConfigManager::load_user_config(const file &file) {
			json_file json_file{};
			//FileからJSONリソースを作成する
			if (!json_file.load(file)) {
				PE_LOG_ERROR("ユーザー設定ファイルをJSONファイルとして読み込むことができませんでした。");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.get_root().get_with_exception<json_object>();
				//Window
				auto window_obj = root_obj->at_with_exception("Window")->get_with_exception<json_object>();
				auto buf_array = *window_obj->at_with_exception("WindowSize")->get_with_exception<std::vector<int>>();
				window_size_.Set(buf_array[0], buf_array[1]);
				is_window_mode_ = *window_obj->at_with_exception("WindowMode")->get_with_exception<bool>();
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
			sys_log.simple_log("ウインドウ(表示)サイズ : ", window_size_.ToString());
			sys_log.simple_log("--------------------------------");
			return true;
		}

		int StandardConfigManager::color_bit_depth() const { return color_bit_depth_; }

		std::string StandardConfigManager::startup_scene_id() const { return startup_scene_id_; }

		std::string StandardConfigManager::game_title() const { return game_title_; }

		std::string StandardConfigManager::game_version_string() const { return game_version_string_; }

		std::array<int, 3> StandardConfigManager::game_version_numbers() const { return game_version_numbers_; }

		bool StandardConfigManager::is_window_mode() const { return is_window_mode_; }

		plnt::Vector2Di StandardConfigManager::draw_size() const { return draw_size_; }

		plnt::Vector2Di StandardConfigManager::window_size() const { return window_size_; }

		const plnt::collision_group_matrix &StandardConfigManager::collision_group_matrix() const {
			return collision_group_matrix_;
		}

		bool StandardConfigManager::is_cursor_visible() const { return is_cursor_visible_; }
	}
}
