#pragma once
#include "object.hpp"
#include <vector>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include "parameter_holder.hpp"
#include "non_copyable.hpp"

namespace plnt {
	namespace private_ {
		class Scene;
	}

	class SceneSetUpProxy;

	/*シーン設定クラス。オブジェクト引継ぎ処理は未実装*/
	class SceneSetUpper : public object, private util::non_copyable<SceneSetUpper> {
	public:
		using super = object;
		SceneSetUpper() = default;
		virtual ~SceneSetUpper() = default;
		/*シーンを初期化する
		@param 対象シーン、初期化パラメータリスト
		*/
		bool InitializeScene(private_::Scene &scene, const util::parameter_holder &initialize_parameters);
		/*シーンを終了する
		@param 対象シーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		util::parameter_holder TerminateScene(private_::Scene &scene, const std::string &next_scene_id,
		                                     const util::parameter_holder &finalize_parameters);

	private:
		/*シーンの設定処理を行う(初期オブジェクト、UI、プロセスの追加、[前シーンからオブジェクトの引継ぎ]未実装)
		@param 設定するシーン、初期化パラメータリスト
		*/
		virtual bool SetUpScene(SceneSetUpProxy &scene_proxy, const util::parameter_holder &initialize_parameters) = 0;
		/*シーンの終了処理を行う(次シーンの初期化パラメータ生成など)
		@param 終了処理するシーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		virtual util::parameter_holder FinalizeScene(SceneSetUpProxy &scene_proxy, const std::string &next_scene_id,
		                                            const util::parameter_holder &finalize_parameters) = 0;
	};

	PE_REFLECTABLE_CLASS(SceneSetUpper);
}

#define PE_SCENE_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
