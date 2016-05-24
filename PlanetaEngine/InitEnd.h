#pragma once
#include <functional>

namespace planeta_engine{
	namespace core{
		namespace init_end{
			/*以下システム関数*/
			bool InitDxLibrary(); //DXライブラリの初期化
			bool EndDxLibrary(); //DXライブラリの終了処理

			bool LoadConfigData(); //設定ファイルを読み込む

			bool SetUpSingletonManagers(); //シングルトンマネージャのセットアップ

			bool InitializeSingletonManagers(); //シングルトンマネージャの初期化
			bool FinalizeSingletonManagers(); //シングルトンマネージャの終了処理

			//各種設定関数(初期化は行わない)
			bool SetUpSystemLog(); //システムログの設定
			bool SetUpFileSystem(); //ファイルシステムの設定。リソースマネージャの設定も行う
			bool SetUpResourceManager(); //リソースマネージャの設定
			//各種終了処理関数(終了処理を行う)
			bool FinalizeSystemLog(); //システムログの終了処理

			bool InitializeDebugSystem(); //デバッグシステムの初期化
			bool FinalizeDebugStstem(); //デバッグシステムの終了処理

			void CreateSystemDirectory(); //必要ならシステム用のディレクトリを作成する
		}
	}
}