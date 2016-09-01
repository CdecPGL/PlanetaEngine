﻿#pragma once
#include "Object.h"
#include <vector>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include "ParameterHolder.h"
#include "SceneSetUpProxy.h" //継承するクラスは必ず使うのでここでインクルード
#include "NonCopyable.h"

namespace planeta {
	namespace core {
		class Scene;
	}
	/*シーン設定クラス。オブジェクト引継ぎ処理は未実装*/
	class SceneSetUpper : public core::Object, private util::NonCopyable<SceneSetUpper> {
	public:
		SceneSetUpper() = default;
		virtual ~SceneSetUpper() = default;
		/*シーンを初期化する
		@param 対象シーン、初期化パラメータリスト
		*/
		bool InitializeScene(core::Scene& scene, const util::ParameterHolder& initialize_parameters);
		/*シーンを終了する
		@param 対象シーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		util::ParameterHolder TerminateScene(core::Scene& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters);
		/*遷移可能SceneIDリスト(からの場合、全てのシーンに遷移可能とみなす)*/
		virtual const std::vector<std::string> GetTransitionableSceneID()const = 0;
		/*指定したシーンに遷移可能か*/
		bool CheckTransitionable(const std::string& scene_id) {
			auto l = GetTransitionableSceneID();
			return l.size() == 0 || std::find(l.begin(), l.end(), scene_id) != l.end();
		}
	private:
		/*シーンの設定処理を行う(初期オブジェクト、UI、プロセスの追加、[前シーンからオブジェクトの引継ぎ]未実装)
		@param 設定するシーン、初期化パラメータリスト
		*/
		virtual bool SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) = 0;
		/*シーンの終了処理を行う(次シーンの初期化パラメータ生成など)
		@param 終了処理するシーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		virtual util::ParameterHolder FinalizeScene(SceneSetUpProxy& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) = 0;
	};
}
