#pragma once
#include "Object.h"
#include <vector>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include "ParameterHolder.h"
namespace planeta_engine{
	namespace core{
		class SceneAccessorForSetUp;
		/*シーン設定クラス。オブジェクト引継ぎ処理は未実装*/
		class SceneSetUpper : public core::Object{
		public:
			SceneSetUpper() = default;
			virtual ~SceneSetUpper() = default;
			/*シーンの設定処理を行う(初期オブジェクト、UI、プロセスの追加、[前シーンからオブジェクトの引継ぎ]未実装)
			@param 設定するシーン、初期化パラメータリスト
			*/
			virtual bool SetUpScene(SceneAccessorForSetUp& scene,const utility::ParameterHolder& initialize_parameters) = 0;
			bool SetUpScene(SceneAccessorForSetUp&& scene, const utility::ParameterHolder& initialize_parameters) { return SetUpScene(scene, initialize_parameters); }
			/*シーンの終了処理を行う(次シーンの初期化パラメータ生成など)
			@param 終了処理するシーン、次のシーンのID、終了処理パラメータリスト
			@return 次のシーンの初期化パラメータ
			*/
			virtual utility::ParameterHolder FinalizeScene(SceneAccessorForSetUp& scene,const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) = 0;
			utility::ParameterHolder FinalizeScene(SceneAccessorForSetUp&& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) { return FinalizeScene(scene, next_scene_id, finalize_parameters); }
			/*使用リソースのタググループリストを返す*/
			virtual const std::vector<std::string> GetUseTagGroups()const = 0;
			/*遷移可能SceneIDリスト(からの場合、全てのシーンに遷移可能とみなす)*/
			virtual const std::vector<std::string> GetTransitionableSceneID()const = 0;
			/*指定したシーンに遷移可能か*/
			bool CheckTransitionable(const std::string& scene_id) {
				auto l = GetTransitionableSceneID();
				return l.size() == 0 ||  std::find(l.begin(), l.end(), scene_id) != l.end();
			}
		private:

		};
	}
}