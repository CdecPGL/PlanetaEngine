#include "InitEnd.h"
#include <windows.h>
#include <fstream>
#include <iostream>

#include "boost/filesystem.hpp"

#include "SystemVariables.h"
#include "ConfigData.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "DebugInformationDisplayer.h"
#include "FileSystemManager.h"
#include "ArchiveManipulator.h"
#include "NormalFolderManipulator.h"
#include "ResourceManager.h"
#include "DrawManager.h"
#include "SoundManager.h"
#include "FileAccessMode.h"

namespace {
	//variables
	std::ofstream LogFileOutPutStream;
}

namespace planeta_engine{
	namespace core{
		namespace init_end{
			bool LoadConfigData() {
				std::string config_file_path(system_variables::SystemDataDirectory + "\\" + system_variables::system_file::SystemConfigFileName);
				return config_data::LoadConfigData(config_file_path);
			}

			bool SetUpSingletonManagers() {
				if (
					SetUpFileSystem()
					//リソースマネージャの設定
					&& SetUpResourceManager()
					) {
					debug::SystemLog::instance().LogMessage("シングルトンマネージャの設定に成功しました。", __FUNCTION__);
					return true;
				}
				else { 
					debug::SystemLog::instance().LogError("シングルトンマネージャの設定に失敗しました。", __FUNCTION__);
					return false;
				}
			}

			bool InitializeSingletonManagers() {
				if (
					FileSystemManager::instance().Initialize()
					&& core::ResourceManager::instance().Initialize()
					&& core::DrawManager::instance().Initialize()
					&& core::SoundManager::instance().Initialize()
					) {
					debug::SystemLog::instance().LogMessage("シングルトンマネージャの初期化に成功しました。", __FUNCTION__);
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("シングルトンマネージャの初期化に失敗しました。",__FUNCTION__);
					return false;
				}
			}

			bool FinalizeSingletonManagers() {
				//サウンドマネージャの終了
				core::SoundManager::instance().Finalize();
				//描画マネージャの終了
				core::DrawManager::instance().Finalize();
				//リソースマネージャの終了
				core::ResourceManager::instance().Finalize();
				//ファイルシステムの終了
				FileSystemManager::instance().Finalize();
				debug::SystemLog::instance().LogMessage("シングルトンマネージャの終了処理を実行しました。", __FUNCTION__);
				return true;
			}

			bool SetUpSystemLog()
			{
				//開発モードならコンソールウインドウを作成し、そこにログを出力
				if (system_variables::DevelopmentMode){
					//コンソール出力を有効化
					debug::SystemLog::instance().ValidateConsoleOutPut();
				}
				//ログ出力ファイルを開く
				LogFileOutPutStream.open(system_variables::LogDirectory + "\\" + system_variables::LogOutPutFileName, std::ios::out | std::ios::trunc);
				//システムログ出力先にファイル出力を追加
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				return true;
			}

			bool FinalizeSystemLog()
			{
				debug::SystemLog::instance().LogMessage("システムログを終了します。", __FUNCTION__);
				//終了
				debug::SystemLog::instance().Finalize();
				//ログファイルを閉じる
				LogFileOutPutStream.close();
				return true;
			}

			bool InitializeDebugSystem()
			{
				//システムログの設定
				if (SetUpSystemLog() == false) { return false; }
				if(debug::SystemCounter::instance().Initialize()
					&& debug::SystemLog::instance().Initialize()) {
					debug::SystemLog::instance().LogMessage("システムログを開始しました。", __FUNCTION__);
				}
				else { return false; }
				//デバッグ情報ウインドウの設定
				if(
					debug::DebugInformationDisplayer::instance().Initialize())
				{
					debug::SystemLog::instance().LogMessage("デバッグ情報ウインドウを準備しました。", __FUNCTION__);
				} else {
					debug::SystemLog::instance().LogError("デバッグ情報ウインドウの準備に失敗しました。", __FUNCTION__);
					return false;
				}
				return true;
			}

			bool FinalizeDebugStstem()
			{
				//デバッグ情報ウインドウの終了処理
				debug::DebugInformationDisplayer::instance().Finalize();
				//システムログの終了処理
				FinalizeSystemLog();
				//システムカウンタの終了処理
				debug::SystemCounter::instance().Finalize();
				return true;
			}

			bool SetUpFileSystem()
			{
				FileSystemManager& flm = FileSystemManager::instance();
				//リソース用ファイルアクセサ設定
				flm.CreateFileAccessor(system_variables::ResourceFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::DevResourceDataFolderPath,false),AccessMode::ReadOnly);
				//GameData用ファイルアクセサ設定
				flm.CreateFileAccessor(system_variables::GameDataFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::SaveDataDirectory,true), AccessMode::ReadWrite);

				debug::SystemLog::instance().Log(debug::LogLevel::Message, __FUNCTION__, "ファイルシステムの設定を行いました。");
				//リソースマネージャの設定も行う
				core::ResourceManager::instance().SetResourceListFileName(core::system_variables::ResourceListFileName);
				return true;
			}

			void CreateSystemDirectory()
			{
				//ログ用フォルダ作成
				boost::filesystem::create_directory(boost::filesystem::path(system_variables::LogDirectory));
				//システムデータ用フォルダ作成
				boost::filesystem::create_directory(boost::filesystem::path(system_variables::SystemDataDirectory));
			}
		}
	}
}