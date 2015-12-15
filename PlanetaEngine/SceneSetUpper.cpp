#include "SceneSetUpper.h"
#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {
		bool SceneSetUpper::InitializeScene(Scene& scene,const utility::ParameterHolder& initialize_parameters) {
			SceneAccessorForSetUp safs(scene);
			//シーンインターフェイスをモジュールに登録
			scene.RegisterSceneInterfaceToModules();
			//シーンデータの準備
			scene.PrepareSceneData();
			//システム設定(特殊プロセスの作成やシーンデータの更新)
			if (!SceneSystemSetUpper()(scene)) {
				debug::SystemLog::instance().LogError("シーンのシステム設定に失敗しました。",__FUNCTION__);
				return false;
			}
			//シーンデータをモジュールに登録
			scene.RegisterSceneDataToModules();
			//固有設定
			if (!SetUpScene(safs, initialize_parameters)) {
				debug::SystemLog::instance().LogError("シーンの固有設定に失敗しました。", __FUNCTION__);
				return false;
			}
			//初期化
			if (!scene.Initialize()) {
				debug::SystemLog::instance().LogError("シーンの初期化に失敗しました。", __FUNCTION__);
				return false;
			}
			return true;
		}

		utility::ParameterHolder SceneSetUpper::TerminateScene(Scene& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) {
			SceneAccessorForSetUp safs(scene);
			auto ret = FinalizeScene(safs, next_scene_id, finalize_parameters); //固有終了処理
			scene.Finalize(); //終了処理
			return ret;
		}

	}
}