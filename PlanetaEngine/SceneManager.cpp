#include "SceneManager.h"
#include "Scene.h"
#include "SceneSetUpper.h"
#include "ResourceManager.h"
#include "SError.h"
#include "SEmpty.h"
#include "SystemLog.h"
#include "NullWeakPointerException.h"
#include "SceneSystemSetUpper.h"
#include "SystemVariables.h"

namespace planeta{
	namespace core{
		SceneManager::SceneManager() :_scene_progress_flag(true), _request(_Request::None), _is_transitioning(false), _is_loading(false), _is_next_scene_loaded(false), _load_progress(0.0) {

		}


		SceneManager::SceneStatus_ SceneManager::Process_()
		{
			try {
				switch (_request)
				{
				case planeta::core::SceneManager::_Request::Transition:
					_transition_proc();
				case planeta::core::SceneManager::_Request::None:
					if (_scene_progress_flag) { _current_scene->Update(); }
					return SceneStatus_::Continue;
				case planeta::core::SceneManager::_Request::Quit:
					return SceneStatus_::Quit;
				case planeta::core::SceneManager::_Request::Error:
					return SceneStatus_::Error;
				default:
					break;
				}
				return SceneStatus_::Error;
			}
			catch (util::NullWeakPointerException& e) {
				PE_LOG_ERROR("Scene::Updateで無効なWeakPointerが参照されました。", e.what());
				return SceneStatus_::Error;
			}
		}

		bool SceneManager::LoadNextScene(const std::string& scene_name){
			if (IsLoading() || IsTransitioning()){ 
				PE_LOG_ERROR("シーンの読み込みに失敗しました。シーンの読み込み中、または遷移中のため新たにシーン(", scene_name, ")を読み込むことはできません。");
				return false;
			} //すでに読み込み中か、遷移中のため、あらたに読み込みできない
			//シーン定義クラス作成
			_next_scene_setupper = _CreateSceneSetUpper(scene_name);
			if (_next_scene_setupper == nullptr) { 
				PE_LOG_ERROR("シーンの読み込みに失敗しました。指定されたシーン(", scene_name, ")のセットアッパーが存在しません。");
				return false; 
			} //シーン定義クラスが作成できなかった
			//現在のシーンから遷移可能なシーンか確認(現在のシーンが空だったら確認しない)
			if (_current_scene!=nullptr && _current_scene_setupper->CheckTransitionable(scene_name) == false) {
				PE_LOG_ERROR("シーンの読み込みに失敗しました。指定されたシーン(", scene_name, ")は現在のシーンから遷移可能なシーンではありません。");
				return false;
			}
			//リソース読み込み
			if (core::ResourceManager::instance().PrepareResources({scene_name}) == false) {
				PE_LOG_WARNING("指定されたシーン(", scene_name, ")のリソース準備に失敗しました。読み込みに失敗したか、対象のリソースが存在しない可能性があります。");
				//return false;
			} //リソースの準備ができなかった
			//準備完了
			_next_scene_id = scene_name;
			_is_next_scene_loaded = true;
			PE_LOG_MESSAGE("指定されたシーン(", scene_name, ")を読み込みました。");
			return true;
		}

		bool SceneManager::TransitionScene(const util::ParameterHolder& transition_parameters)
		{
			if (IsTransitionable()==false){
				PE_LOG_ERROR("シーン遷移予約に失敗しました。遷移中のため、新たに遷移処理をはじめることはできません。");
				return false;
			} //遷移処理を始めることはできない
			//シーン遷移要求
			_request = _Request::Transition;
			_transition_parameters = transition_parameters;
			PE_LOG_MESSAGE("シーン遷移予約を行いました。");
			return true;
		}

		void SceneManager::_transition_proc()
		{
			//現在のシーンを終了
			util::ParameterHolder next_scene_initialize_parameters = _end_current_scene();
			//新しいシーンを作成
			std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();
			//新しいシーンを初期化
			if (!InitializeScene_(*new_scene,*_next_scene_setupper,next_scene_initialize_parameters)) {
				PE_LOG_ERROR("シーン遷移に失敗しました。新しいシーン(", _next_scene_id, ")のセットアップまたは初期化に失敗しました。");
				_transition_to_error_scene();
				return;
			}
			_current_scene = std::move(new_scene);
			_current_scene_setupper = std::move(_next_scene_setupper);
			//未使用リソースを削除(シーンの更新前のため、ここで削除してよい)
			core::ResourceManager::instance().UnloadUnusedResouces();
			//リクエストと準備状況をリセット
			_request = _Request::None;
			_is_next_scene_loaded = false;
			PE_LOG_MESSAGE("シーン(", _next_scene_id, ")に遷移しました。");
			_next_scene_id = "";
		}

		bool SceneManager::Initialize()
		{
			//空のシーンをセット
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<SEmpty>();
			std::shared_ptr<Scene> es = std::make_shared<Scene>();
			InitializeScene_(*es, *ecd, util::ParameterHolder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;

			return true;
		}

		bool SceneManager::Finalize()
		{
			//現在のシーンを終了
			_end_current_scene();
			return true;
		}

		std::shared_ptr<SceneSetUpper> SceneManager::_CreateSceneSetUpper(const std::string& scene_name)
		{
			//シーン名にプレフィックスをつけたクラスを作成。
			auto setupper = Reflection::CreateObjectByID<SceneSetUpper>(system_variables::prefixes::Scene + scene_name);
			return setupper;
		}

		void SceneManager::_transition_to_error_scene()
		{
			PE_LOG_ERROR("エラーシーンに遷移します。");
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<SError>();
			std::shared_ptr<Scene> es = std::make_shared<Scene>();
			InitializeScene_(*es, *ecd, util::ParameterHolder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;
		}

		util::ParameterHolder SceneManager::_end_current_scene()
		{
			//現在のシーンを終了(現在のシーンがなかったら空のパラメータを格納)
			if (_current_scene) {
				return FinalizeScene_(*_current_scene, *_current_scene_setupper, _next_scene_id, _transition_parameters);
			}
			else { return util::ParameterHolder(); }
		}

		void SceneManager::SetCollisionGroupMatrix_(std::shared_ptr<CollisionGroupMatrix>&& cg_matrix) {
			collision_group_matrix_ = std::move(cg_matrix);
		}

		bool SceneManager::InitializeScene_(Scene& scene, SceneSetUpper& setupper, const util::ParameterHolder& init_param) {
			SceneSetUpProxy safs(scene);
			//シーンデータの準備
			scene.SetCollisionGroupMatrix(collision_group_matrix_);
			scene.PrepareSceneData(this);
			//システム設定(特殊プロセスの作成やシーンデータの更新)
			if (!core::SceneSystemSetUpper()(scene)) {
				PE_LOG_ERROR("シーンのシステム設定に失敗しました。");
				return false;
			}
			//シーンデータをモジュールに登録
			scene.RegisterSceneDataToModules();
			//固有設定
			if (!setupper.InitializeScene(scene, init_param)) {
				PE_LOG_ERROR("シーンの固有設定に失敗しました。");
				return false;
			}
			//初期化
			if (!scene.Initialize()) {
				PE_LOG_ERROR("シーンの初期化に失敗しました。");
				return false;
			}
			return true;
		}

		util::ParameterHolder SceneManager::FinalizeScene_(core::Scene& scene, SceneSetUpper& setupper, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) {
			SceneSetUpProxy safs(scene);
			auto ret = setupper.TerminateScene(scene,next_scene_id, finalize_parameters); //固有終了処理
			scene.Finalize(); //終了処理
			return ret;
		}

	}
}