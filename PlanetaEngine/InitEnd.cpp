#include "InitEnd.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include "SystemVariables.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "FileLoadManager.h"
#include "ArchiveLoader.h"
#include "NormalFolderLoader.h"
#include "ResourceManager.h"
#include "DrawManager.h"
#include "SoundManager.h"

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
					debug::SystemLog::instance().LogMessage("シングルトンマネージャの設定に成功しました。", "SetUpSingletoneManagers");
					return true;
				}
				else { 
					debug::SystemLog::instance().LogError("シングルトンマネージャの設定に失敗しました。", "SetUpSingletoneManagers");
					return false;
				}
			}

			bool InstantiateSingletonManagers() {
				if (
					file_system::FileLoadManager::Instantiate()
					&& core::ResourceManager::Instantiate()
					&& core::DrawManager::Instantiate()
					&& core::SoundManager::Instantiate()
					) {
					debug::SystemLog::instance().LogMessage("シングルトンマネージャのインスタンス化に成功しました。", "InstantiateSingletonManagers");
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("シングルトンマネージャのインスタンス化に失敗しました。", "InstantiateSingletonManagers");
					return false;
				}
			}

			bool DisposeSingletonManagers() {
				core::SoundManager::Dispose();
				core::DrawManager::Dispose();
				core::ResourceManager::Dispose();
				file_system::FileLoadManager::Dispose();
				debug::SystemLog::instance().LogMessage("シングルトンマネージャのインスタンスを破棄しました。", "DisposeSingletonManagers");
				return true;
			}

			bool InitializeSingletonManagers() {
				if (
					file_system::FileLoadManager::instance().Initialize()
					&& core::ResourceManager::instance().Initialize()
					&& core::DrawManager::instance().Initialize()
					&& core::SoundManager::instance().Initialize()
					) {
					debug::SystemLog::instance().LogMessage("シングルトンマネージャの初期化に成功しました。", "InitializeSingletonManagers");
					return true;
				}
				else {
					debug::SystemLog::instance().LogError("シングルトンマネージャの初期化に失敗しました。", "InitializeSingletonManagers");
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
				file_system::FileLoadManager::instance().Finalize();
				debug::SystemLog::instance().LogMessage("シングルトンマネージャの終了処理を実行しました。", "FinalizeSingletonManagers");
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
				LogFileOutPutStream.open(system_variables::LogOutPutFileName);
				//システムログ出力先にファイル出力を追加
				debug::SystemLog::instance().AddLogOutStream(LogFileOutPutStream);
				return true;
			}

			bool FinalizeSystemLog()
			{
				debug::SystemLog::instance().LogMessage("システムログを終了します。", "FinalizeSystemLog");
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
					debug::SystemLog::instance().LogMessage("システムログを開始しました。", "InitializeDebugSystem");
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
				FileLoadManager& flm = FileLoadManager::instance();
				flm.PushLoader(std::make_shared<ArchiveLoader>(system_variables::ResourceDataArchiveFilePath, system_variables::ResourceDataArchiveDecryptionKey));
				//開発モード時は、開発用Resourceフォルダと、テスト用Resourceフォルダも追加する。
				bool dev_mode = false;
				if (system_variables::DevelopmentMode) {
					flm.PushLoader(std::make_shared<NormalFolderLoader>(system_variables::DevResourceDataFolderPath));
					flm.PushLoader(std::make_shared<NormalFolderLoader>(system_variables::TestResourceDataFolderPath));
					dev_mode = true;
				}
				char str[256];
				sprintf_s(str, 256, "ファイルシステムの設定を行いました。(DevelopmentMode=%s)", dev_mode ? "true" : "false");
				debug::SystemLog::instance().LogMessage(str, "InitializeDebugSystem");
				//リソースマネージャの設定も行う
				core::ResourceManager::instance().SetResourceListFileName(core::system_variables::ResourceListFileName);
				return true;
			}
		}
	}
}