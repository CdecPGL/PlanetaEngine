#include "InitEnd.h"
#include <windows.h>
#include <fstream>
#include <iostream>

#include "boost/filesystem.hpp"

#include "SystemVariables.h"
#include "SystemLog.h"
#include "SystemCounter.h"
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

			bool InstantiateSingletonManagers() {
				if (
					file_system::FileSystemManager::Instantiate()
					&& core::ResourceManager::Instantiate()
					&& core::DrawManager::Instantiate()
					&& core::SoundManager::Instantiate()
					) {
					debug::SystemLog::instance().LogMessage("シングルトンマネージャのインスタンス化に成功しました。", __FUNCTION__);
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("シングルトンマネージャのインスタンス化に失敗しました。", __FUNCTION__);
					return false;
				}
			}

			bool DisposeSingletonManagers() {
				core::SoundManager::Dispose();
				core::DrawManager::Dispose();
				core::ResourceManager::Dispose();
				file_system::FileSystemManager::Dispose();
				debug::SystemLog::instance().LogMessage("シングルトンマネージャのインスタンスを破棄しました。", __FUNCTION__);
				return true;
			}

			bool InitializeSingletonManagers() {
				if (
					file_system::FileSystemManager::instance().Initialize()
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
				file_system::FileSystemManager::instance().Finalize();
				debug::SystemLog::instance().LogMessage("シングルトンマネージャの終了処理を実行しました。", __FUNCTION__);
				return true;
			}

			bool SetUpSystemLog()
			{
				//開発モードならコンソールウインドウを作成し、そこにログを出力
				if (system_variables::DevelopmentMode){
					//コンソールウインドウを標準入出力に割り当て
					AllocConsole();
					FILE* fp;
					freopen_s(&fp, "CONOUT$", "w", stdout);
					freopen_s(&fp, "CONIN$", "r", stdin);
					//システムログ出力先に標準入出力を追加
					debug::SystemLog::instance().AddLogOutStream(std::cout);
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
				if (system_variables::DevelopmentMode) {
					//コンソール解放
					FreeConsole();
				}
				return true;
			}

			bool InitializeDebugSystem()
			{
				if(debug::SystemCounter::Instantiate()
					&& debug::SystemLog::Instantiate()) {
				}
				else { return false; }
				//システムログの設定
				if (SetUpSystemLog() == false) { return false; }
				if(debug::SystemCounter::instance().Initialize()
					&& debug::SystemLog::instance().Initialize()) {
					debug::SystemLog::instance().LogMessage("システムログを開始しました。", __FUNCTION__);
				}
				else { return false; }
				return true;
			}

			bool FinalizeDebugStstem()
			{
				//システムログの終了処理
				FinalizeSystemLog();
				//システムカウンタの終了処理
				debug::SystemCounter::instance().Finalize();
				//破棄
				debug::SystemLog::Dispose();
				debug::SystemCounter::Dispose();
				return true;
			}

			bool SetUpFileSystem()
			{
				using namespace file_system;
				FileSystemManager& flm = FileSystemManager::instance();
				//リソース用ファイルアクセサ設定
				flm.CreateFileAccessor(system_variables::ResourceFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::DevResourceDataFolderPath,false),file_system::AccessMode::ReadOnly);
				//GameData用ファイルアクセサ設定
				flm.CreateFileAccessor(system_variables::GameDataFileAccessorID, std::make_shared<NormalFolderManipulator>(system_variables::SaveDataDirectory,true), file_system::AccessMode::ReadWrite);

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