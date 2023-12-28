#include "StandardConfigManager.hpp"
#include "JsonFile.hpp"
#include "LogUtility.hpp"
#include "StringUtility.hpp"

namespace plnt {
	namespace private_ {
		bool StandardConfigManager::load_system_config(const file &file) {
			JsonFile json_file{};
			//FileからJSONリソースを作成する
			if (!json_file.Load(file)) {
				PE_LOG_ERROR("システム設定ファイルをJSONファイルとして読み込むことができませんでした。");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.GetRoot().GetWithException<JSONObject>();
				//Game
				auto game_obj = root_obj->AtWithException("Game")->GetWithException<JSONObject>();
				game_title_ = *game_obj->AtWithException("Title")->GetWithException<std::string>();
				auto buf_array = *game_obj->AtWithException("Version")->GetWithException<std::vector<int>>();
				game_version_numbers_[0] = buf_array[0];
				game_version_numbers_[1] = buf_array[1];
				game_version_numbers_[2] = buf_array[2];
				//Engine
				auto engine_obj = root_obj->AtWithException("Engine")->GetWithException<JSONObject>();
				color_bit_depth_ = *engine_obj->AtWithException("ColorBitDepth")->GetWithException<int>();
				buf_array = *engine_obj->AtWithException("DrawSize")->GetWithException<std::vector<int>>();
				draw_size_.Set(buf_array[0], buf_array[1]);
				is_cursor_visible_ = *engine_obj->AtWithException("IsCursorVisible")->GetWithException<bool>();
				//Program
				auto prog_obj = root_obj->AtWithException("Program")->GetWithException<JSONObject>();
				auto scene_obj = prog_obj->AtWithException("Scene")->GetWithException<JSONObject>();
				//-シーン
				startup_scene_id_ = *scene_obj->AtWithException("Startup")->GetWithException<std::string>();
				//-衝突システム
				auto col_obj = prog_obj->AtWithException("Collision")->GetWithException<JSONObject>();
				//--衝突グループ
				std::vector<std::string> group_list;
				group_list = *col_obj->AtWithException("Groups")->GetWithException<std::vector<std::string>>();
				collision_group_matrix_.add_collision_groups(group_list);
				//--衝突可能マトリックス
				auto col_mtx = *col_obj->AtWithException("CollidableMatrix")->GetWithException<std::unordered_map<
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
			} catch (JSONTypeError &e) {
				PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。型が間違えている可能性があります。(", e.what(), ")");
				return false;
			}
			//設定データ取得成功
			//バージョン文字列設定
			game_version_string_ = util::ConvertAndConnectToString(game_version_numbers_[0], ".",
			                                                       game_version_numbers_[1], ".",
			                                                       game_version_numbers_[2]);
			//設定をログに出力
			auto &sys_log = *Game::instance().log_manager();
			PE_LOG_MESSAGE("システム設定を読み込みました。");
			sys_log.SimpleLog("--------システム設定情報--------");
			sys_log.SimpleLog("ゲームタイトル : ", game_title_);
			sys_log.SimpleLog("ゲームバージョン : ", game_version_string_);
			sys_log.SimpleLog("カラービット深度 : ", color_bit_depth_);
			sys_log.SimpleLog("描画(内部)サイズ : ", draw_size_.ToString());
			sys_log.SimpleLog("--------------------------------");
			return true;
		}

		bool StandardConfigManager::load_user_config(const file &file) {
			JsonFile json_file{};
			//FileからJSONリソースを作成する
			if (!json_file.Load(file)) {
				PE_LOG_ERROR("ユーザー設定ファイルをJSONファイルとして読み込むことができませんでした。");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.GetRoot().GetWithException<JSONObject>();
				//Window
				auto window_obj = root_obj->AtWithException("Window")->GetWithException<JSONObject>();
				auto buf_array = *window_obj->AtWithException("WindowSize")->GetWithException<std::vector<int>>();
				window_size_.Set(buf_array[0], buf_array[1]);
				is_window_mode_ = *window_obj->AtWithException("WindowMode")->GetWithException<bool>();
			} catch (std::out_of_range &e) {
				PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。内容が不足している可能性があります。(", e.what(), ")");
				return false;
			} catch (JSONTypeError &e) {
				PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。型が間違えている可能性があります。(", e.what(), ")");
				return false;
			}
			//設定データ取得成功
			//設定をログに出力
			auto &sys_log = *Game::instance().log_manager();
			PE_LOG_MESSAGE("ユーザー設定を読み込みました。");
			sys_log.SimpleLog("--------ユーザー設定情報--------");
			sys_log.SimpleLog("ウインドウモード : ", is_window_mode_ ? "有効" : "無効");
			sys_log.SimpleLog("ウインドウ(表示)サイズ : ", window_size_.ToString());
			sys_log.SimpleLog("--------------------------------");
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
