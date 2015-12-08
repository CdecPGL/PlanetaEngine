#include "boost/lexical_cast.hpp"
#include "ConfigData.h"
#include "INIFileResource.h"
#include "FileSystemUtility.h"
#include "MakeResource.h"

namespace planeta_engine {
	namespace core {
		namespace config_data {
			std::string game::GameTitle_("NULL"); //ゲームタイトル
			std::string game::VersionString_("0.0.0"); //バージョン文字列
			int game::MajorVersionNumber_(0);
			int game::MinorVersionNumber_(0);
			int game::SubVersionNumber_(0);

			bool engine::WindowMode_(true); //ウインドウモードか
			int engine::ColorBitDepth_(32); //カラービット深度
			math::Vector2Di engine::DrawSize_(640, 480); //描画ザイズ
			math::Vector2Di engine::WindowSize_(640, 480); //ウインドウサイズ
			unsigned int engine::ResourceDecryptionKey_(0);

			bool user::WindowSizeConfigurable_(false); //ウインドウサイズをユーザーが設定可能か
			bool user::WindowModeConfigurable_(false); //ウインドウモードか否かをユーザーが設定可能か

			namespace {
				using namespace planeta_engine::debug;
				using namespace planeta_engine::resources;
				using SectionDataType = std::pair<std::string, typename INIFileResource::SectionType>;
				bool GetSection(const INIFileResource& ini, const std::string& section_name, SectionDataType& dst) {
					auto it = ini.find(section_name);
					if (it == ini.end()) {
						SystemLog::instance().Log(LogLevel::Error, __FUNCTION__, "セクション\"", section_name, "\"が存在しません。");
						return false;
					}
					dst = *it;
					return true;
				}

				bool SetConfigData(const SectionDataType& section, const std::string& parameter_name, bool& dst) {
					auto it = section.second.find(parameter_name);
					if (it == section.second.end()) {
						SystemLog::instance().Log(LogLevel::Error, __FUNCTION__, "セクション\"", section.first, "\"にパラメータ\"", parameter_name, "\"が存在しません。");
						return false;
					}
					if (it->second[0] == 't' || it->second[0] == 'T') { dst = true; } //一文字目がtだったらtrue
					else if (it->second[0] == 'f' || it->second[0] == 'F') { dst = false; } //一文字目がfだったらfalse
					else { //それ以外はキャストして判断
						try {
							dst = boost::lexical_cast<bool>(it->second);
						} catch (boost::bad_lexical_cast&) {
							SystemLog::instance().Log(LogLevel::Error, __FUNCTION__, "セクション\"", section.first, "\"のパラメータ\"", parameter_name, "\"の値が不正です。");
							return false;
						}
					}
					return true;
				}

				template<typename T>
				bool SetConfigData(const SectionDataType& section, const std::string& parameter_name, T& dst) {
					auto it = section.second.find(parameter_name);
					if (it == section.second.end()) {
						SystemLog::instance().Log(LogLevel::Error, __FUNCTION__, "セクション\"", section.first, "\"にパラメータ\"", parameter_name, "\"が存在しません。");
						return false;
					}
					try {
						dst = boost::lexical_cast<T>(it->second);
					} catch (boost::bad_lexical_cast&) {
						SystemLog::instance().Log(LogLevel::Error, __FUNCTION__, "セクション\"", section.first, "\"のパラメータ\"", parameter_name, "\"の値が不正です。");
						return false;
					}
					return true;
				}
			}

			bool LoadConfigData(const std::string& file_path) {
				auto ini_res = MakeResource<resources::INIFileResource>();
				//標準ファイル入力からFileを作成
				std::shared_ptr<file_system::File> file = file_system::utility::CreateFileFromStandardFileInput(file_path);
				if (file == nullptr) {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, "設定ファイル\"", file_path, "\"を読み込めませんでした", __FUNCTION__);
					return false;
				}
				//Fileを復号化する

				//FileからINIリソースを作成する
				if (!ini_res->Create(file)) {
					debug::SystemLog::instance().LogError("設定ファイルをINIファイルとして読み込むことができませんでした。", __FUNCTION__);
					return false;
				}
				//各種設定データを取得する
				SectionDataType section;
				if (GetSection(*ini_res, "Game", section)
					&& SetConfigData(section, "GameTitle", game::GameTitle_)
					&& SetConfigData(section, "MajorVersion", game::MajorVersionNumber_)
					&& SetConfigData(section, "MinorVersion", game::MinorVersionNumber_)
					&& SetConfigData(section, "SubVersion", game::SubVersionNumber_)
					&& GetSection(*ini_res, "Engine", section)
					&& SetConfigData(section, "WindowMode", engine::WindowMode_)
					&& SetConfigData(section, "ColorBitDepth", engine::ColorBitDepth_)
					&& SetConfigData(section, "DrawWidth", engine::DrawSize_.x)
					&& SetConfigData(section, "DrawHeight", engine::DrawSize_.y)
					&& SetConfigData(section, "WindowWidth", engine::WindowSize_.x)
					&& SetConfigData(section, "WindowHeight", engine::WindowSize_.y)
					&& SetConfigData(section, "ResourceDecryptionKey", engine::ResourceDecryptionKey_)
					&& GetSection(*ini_res, "User", section)
					&& SetConfigData(section, "WindowSizeConfigurable", user::WindowSizeConfigurable_)
					&& SetConfigData(section, "WindowModeConfigurable", user::WindowModeConfigurable_)
					) { //設定データ取得成功
						//バージョン文字列設定
						game::VersionString_ =
							boost::lexical_cast<std::string>(game::MajorVersionNumber())
							+ "." + boost::lexical_cast<std::string>(game::MinorVersionNumber())
							+ "." + boost::lexical_cast<std::string>(game::SubVersionNumber());
					//設定をログに出力
					using namespace debug;
					auto& sys_log = SystemLog::instance();
					sys_log.Log(LogLevel::Message, __FUNCTION__, "システム設定を読み込みました。");
					sys_log.SimpleLog("--------システム設定情報--------");
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
					debug::SystemLog::instance().LogError("設定ファイルからデータを取得することができませんでした。", __FUNCTION__);
					return false;
				}
				return true;
			}
		};
	}
}