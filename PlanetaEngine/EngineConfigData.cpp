#include "boost/lexical_cast.hpp"
#include "EngineConfigData.h"
#include "JSONResource.h"
#include "FileSystemUtility.h"
#include "INIFileResource.h"
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

			namespace {
				using namespace planeta::debug;
				using namespace planeta::resources;
				template<typename T>
				bool SetValueFromJSONObject(const JSONObject& json_obj, const std::string& key, T& dst) {
					//bool以外の数値型はdoubleとして扱う
					using TT = std::conditional_t<std::is_same<T, bool>::value, bool,
						std::conditional_t<std::is_arithmetic<T>::value, double, T>>;
					decltype(auto) val = json_obj.At<TT>(key);
					if (val) {
						dst = static_cast<T>(*val);
						return true;
					} else {
						PE_LOG_ERROR("キー\"", key, "\"の値が取得できませんでした。");
						return false;
					}
				}
				template<typename T>
				bool SetCRefOptFromJSONObject(const JSONObject& json_obj, const std::string& key,  boost::optional<const T&>& dst) {
					decltype(auto) val = json_obj.At<T>(key);
					if (val) {
						dst = val;
						return true;
					} else {
						PE_LOG_ERROR("キー\"", key, "\"の値が取得できませんでした。");
						return false;
					}
				}
				bool SetVector2DiFromJSONArray(const JSONArray& json_ary,Vector2Di& dst) {
					decltype(auto) opt = json_ary.At<double>(0);
					if (opt) {
						dst.x = static_cast<int>(*opt);
					} else {
						PE_LOG_ERROR("JSONArray[0]をVector2Di.xに変換できませんでした。");
						return false;
					}
					opt = json_ary.At<double>(1);
					if (opt) {
						dst.y = static_cast<int>(*opt);
					} else {
						PE_LOG_ERROR("JSONArray[1]をVector2Di.yに変換できませんでした。");
						return false;
					}
					return true;
				}
				template<typename T,int N>
				bool SetDataToPointerArrayFromJSONArray(const JSONArray& json_ary, const std::array<T*, N>& ary) {
					//bool以外の数値型はdoubleとして扱う
					using TT = std::conditional_t<std::is_same<T, bool>::value, bool,
						std::conditional_t<std::is_arithmetic<T>::value, double, T>>;
					for (int i = 0; i < N; ++i) {
						decltype(auto) opt = json_ary.At<TT>(i);
						if (opt) {
							*ary[i] = static_cast<T>(*opt);
						} else {
							PE_LOG_ERROR("JSONArray[", i, "]をstd::array<N,T*>[", i, "]にセットできませんでした。");
							return false;
						}
					}
					return false;
				}
			}

			bool LoadConfigData(const std::shared_ptr<File>& file) {
				assert(file != nullptr && file->GetStatus() == File::FileStatus::Available);

				auto json_res = MakeResource<resources::JSONResource>();
				//FileからINIリソースを作成する
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("設定ファイルをJSONファイルとして読み込むことができませんでした。");
					return false;
				}
				decltype(auto) root_obj = json_res->GetRoot().Get<JSONObject>();
				boost::optional<const JSONObject&> game_obj;
				boost::optional<const JSONObject&> engine_obj;
				boost::optional<const JSONObject&> user_obj;
				boost::optional<const JSONArray&> buffer_array;
				if (SetCRefOptFromJSONObject(*root_obj, "Game", game_obj)
					&& SetValueFromJSONObject(*game_obj, "Title", game::GameTitle_)
					&& SetCRefOptFromJSONObject(*game_obj, "Version", buffer_array)
					&& SetDataToPointerArrayFromJSONArray(*buffer_array, std::array<int*,3>{&game::MajorVersionNumber_,&game::MinorVersionNumber_,&game::SubVersionNumber_})
					&& SetCRefOptFromJSONObject(*root_obj, "Engine", engine_obj)
					&& SetValueFromJSONObject(*engine_obj, "WindowMode", engine::WindowMode_)
					&& SetValueFromJSONObject(*engine_obj, "ColorBitDepth", engine::ColorBitDepth_)
					&& SetCRefOptFromJSONObject(*engine_obj, "DrawSize", buffer_array)
					&& SetVector2DiFromJSONArray(*buffer_array,engine::DrawSize_)
					&& SetCRefOptFromJSONObject(*engine_obj, "WindowSize", buffer_array)
					&& SetVector2DiFromJSONArray(*buffer_array, engine::WindowSize_)
					&& SetValueFromJSONObject(*engine_obj, "ResourceDecryptionKey", engine::ResourceDecryptionKey_)
					&& SetCRefOptFromJSONObject(*root_obj, "User", user_obj)
					&& SetValueFromJSONObject(*user_obj, "WindowSizeConfigurable", user::WindowSizeConfigurable_)
					&& SetValueFromJSONObject(*user_obj, "WindowModeConfigurable", user::WindowModeConfigurable_)
					) { //設定データ取得成功
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
				} else { //設定データ取得失敗
					PE_LOG_ERROR("設定ファイルからデータを取得することができませんでした。");
					return false;
				}
				return true;
			}
		};
	}
}