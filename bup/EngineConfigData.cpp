#include "boost/lexical_cast.hpp"
#include "EngineConfigData.h"
#include "RJson.h"
#include "FileSystemUtility.h"
#include "RIni.h"
#include "MakeResource.h"
#include "File.h"

namespace planeta {
	namespace core {
		namespace engine_config {
			std::string game::GameTitle_("NULL"); //ゲームタイトル
			std::string game::VersionString_("0.0.0"); //バージョン文字列
			int game::MajorVersionNumber_(0);
			int game::MinorVersionNumber_(0);
			int game::SubVersionNumber_(0);

			bool engine::WindowMode_(true); //ウインドウモードか
			int engine::ColorBitDepth_(32); //カラービット深度
			Vector2Di engine::DrawSize_(640, 480); //描画ザイズ
			Vector2Di engine::WindowSize_(640, 480); //ウインドウサイズ
			unsigned int engine::ResourceDecryptionKey_(0);

			bool user::WindowSizeConfigurable_(false); //ウインドウサイズをユーザーが設定可能か
			bool user::WindowModeConfigurable_(false); //ウインドウモードか否かをユーザーが設定可能か

			bool LoadConfigData(const std::shared_ptr<File>& file) {
				assert(file != nullptr && file->GetStatus() == File::FileStatus::Available);

				auto json_res = MakeResource<RJson>();
				//FileからJSONリソースを作成する
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("設定ファイルをJSONファイルとして読み込むことができませんでした。");
					return false;
				}
				try {
					decltype(auto) root_obj = json_res->GetRoot().GetWithException<JSONObject>();
					auto game_obj = root_obj->AtWithException("Game")->GetWithException<JSONObject>();
					auto engine_obj = root_obj->AtWithException("Engine")->GetWithException<JSONObject>();
					auto user_obj = root_obj->AtWithException("User")->GetWithException<JSONObject>();
					game::GameTitle_ = *game_obj->AtWithException("Title")->GetWithException<std::string>();
					auto buf_array = *game_obj->AtWithException("Version")->GetWithException<std::vector<int>>();
					game::MajorVersionNumber_ = buf_array[0];
					game::MinorVersionNumber_ = buf_array[1];
					game::SubVersionNumber_ = buf_array[2];
					engine::WindowMode_ = *engine_obj->AtWithException("WindowMode")->GetWithException<bool>();
					engine::ColorBitDepth_ = *engine_obj->AtWithException("ColorBitDepth")->GetWithException<int>();
					buf_array = *engine_obj->AtWithException("DrawSize")->GetWithException<std::vector<int>>();
					engine::DrawSize_.Set(buf_array[0], buf_array[1]);
					buf_array = *engine_obj->AtWithException("WindowSize")->GetWithException<std::vector<int>>();
					engine::WindowSize_.Set(buf_array[0], buf_array[1]);
					engine::ResourceDecryptionKey_ = *engine_obj->AtWithException("ColorBitDepthResourceDecryptionKey")->GetWithException<int>();
					user::WindowSizeConfigurable_ = *user_obj->AtWithException("WindowSizeConfigurable")->GetWithException<bool>();
					user::WindowModeConfigurable_ = *user_obj->AtWithException("WindowSModeConfigurable")->GetWithException<bool>();
				} catch (std::out_of_range& e) {
					PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。内容が不足している可能性があります。(", e.what(), ")");
					return false;
				} catch (JSONTypeError& e) {
					PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。型が間違えている可能性があります。(", e.what(), ")");
					return false;
				}
				//設定データ取得成功
				//バージョン文字列設定
				game::VersionString_ =
					boost::lexical_cast<std::string>(game::MajorVersionNumber())
					+ "." + boost::lexical_cast<std::string>(game::MinorVersionNumber())
					+ "." + boost::lexical_cast<std::string>(game::SubVersionNumber());
				//設定をログに出力
				using namespace debug;
				auto& sys_log = SystemLog::instance();
				PE_LOG_MESSAGE("エンジン設定を読み込みました。");
				sys_log.SimpleLog("--------エンジン設定情報--------");
				sys_log.SimpleLog("ゲームタイトル : ", game::GameTitle());
				sys_log.SimpleLog("ゲームバージョン : ", game::VersionString());
				sys_log.SimpleLog("ウインドウモード : ", engine::WindowMode() ? "有効" : "無効");
				sys_log.SimpleLog("カラービット深度 : ", engine::ColorBitDepth());
				sys_log.SimpleLog("描画(内部)サイズ : ", engine::DrawSize().ToString());
				sys_log.SimpleLog("ウインドウ(表示)サイズ : ", engine::WindowSize().ToString());
				sys_log.SimpleLog("ユーザーによるウインドウ(表示)サイズの設定 : ", user::WindowSizeConfigurable() ? "有効" : "無効");
				sys_log.SimpleLog("ユーザーによるウインドウモードの設定 : ", user::WindowModeConfigurable() ? "有効" : "無効");
				sys_log.SimpleLog("--------------------------------");
				return true;
			}
		};
	}
}