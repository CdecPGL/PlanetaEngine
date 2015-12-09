#include "SceneManager.h"
#include "Scene.h"
#include "SceneSetUpper.h"
#include "ResourceManager.h"
#include "ErrorSceneDefinition.h"
#include "EmptySceneDefinition.h"
#include "SceneSystemSetUpper.h"
#include "SystemLog.h"
#include "NullWeakPointerException.h"
#include "SceneAccessorForSetUp.h"

namespace planeta_engine{
	namespace core{
		SceneManager::SceneManager(IGameAccessor& engine) :_game(engine),_scene_progress_flag(true), _request(_Request::None), _is_transitioning(false), _is_loading(false), _is_next_scene_loaded(false), _load_progress(0.0) {

		}


		SceneManager::SceneStatus SceneManager::Process()
		{
			try {
				switch (_request)
				{
				case planeta_engine::core::SceneManager::_Request::Transition:
					_transition_proc();
				case planeta_engine::core::SceneManager::_Request::None:
					if (_scene_progress_flag) { _current_scene->Update(); }
					return SceneStatus::Continue;
				case planeta_engine::core::SceneManager::_Request::Quit:
					return SceneStatus::Quit;
				case planeta_engine::core::SceneManager::_Request::Error:
					return SceneStatus::Error;
				default:
					break;
				}
				return SceneStatus::Error;
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("Scene::Updateで無効なWeakPointerが参照されました。") + e.what(), "SceneManager::Process");
				return SceneStatus::Error;
			}
		}

		bool SceneManager::LoadNextScene(const std::string& scene_name){
			if (IsLoading() || IsTransitioning()){ 
				debug::SystemLog::instance().LogError(std::string("シーンの読み込みに失敗しました。シーンの読み込み中、または遷移中のため新たにシーン(") + scene_name + ")を読み込むことはできません。", "SceneManager::LoadNextScene");
				return false;
			} //すでに読み込み中か、遷移中のため、あらたに読み込みできない
			//シーン定義クラス作成
			_next_scene_setupper = _CreateSceneSetUpper(scene_name);
			if (_next_scene_setupper == nullptr) { 
				debug::SystemLog::instance().LogError(std::string("シーンの読み込みに失敗しました。指定されたシーン(") + scene_name + ")のセットアッパーが存在しません。", "SceneManager::LoadNextScene");
				return false; 
			} //シーン定義クラスが作成できなかった
			//現在のシーンから遷移可能なシーンか確認(現在のシーンが空だったら確認しない)
			if (_current_scene!=nullptr && _current_scene_setupper->CheckTransitionable(scene_name) == false) {
				debug::SystemLog::instance().LogError(std::string("シーンの読み込みに失敗しました。指定されたシーン(") + scene_name + ")は現在のシーンから遷移可能なシーンではありません。", "SceneManager::LoadNextScene");
				return false;
			}
			//リソース読み込み
			const std::vector<std::string>& ntags = _next_scene_setupper->GetUseTagGroups();
			if (core::ResourceManager::instance().PrepareResources(ntags) == false) {
				debug::SystemLog::instance().LogError(std::string("シーンの読み込みに失敗しました。指定されたシーン(") + scene_name + ")のリソース準備に失敗しました。", "SceneManager::LoadNextScene");
				return false; 
			} //リソースの準備ができなかった
			//準備完了
			_next_scene_id = scene_name;
			_is_next_scene_loaded = true;
			debug::SystemLog::instance().LogMessage(std::string("指定されたシーン(") + scene_name + ")を読み込みました。", "SceneManager::LoadNextScene");
			return true;
		}

		bool SceneManager::TransitionScene(const utility::ParameterHolder& transition_parameters)
		{
			if (IsTransitionable()==false){
				debug::SystemLog::instance().LogError("シーン遷移予約に失敗しました。遷移中のため、新たに遷移処理をはじめることはできません。", "SceneManager::TransitionScene");
				return false;
			} //遷移処理を始めることはできない
			//シーン遷移要求
			_request = _Request::Transition;
			_transition_parameters = transition_parameters;
			debug::SystemLog::instance().LogMessage("シーン遷移予約を行いました。", "SceneManager::TransitionScene");
			return true;
		}

		void SceneManager::_transition_proc()
		{
			//現在のシーンを終了
			utility::ParameterHolder next_scene_initialize_parameters = _end_current_scene();
			//新しいシーンを作成
			std::shared_ptr<Scene> new_scene = Scene::MakeShared(_game);
			SceneSystemSetUpper sssu; //シーンのシステムセットアッパー(特殊プロセスの設定)
			if (!(sssu(*new_scene) && _next_scene_setupper->SetUpScene(SceneAccessorForSetUp(*new_scene), next_scene_initialize_parameters))) {
				debug::SystemLog::instance().LogError(std::string("シーン遷移に失敗しました。新しいシーン(") + _next_scene_id + ")のセットアップに失敗しました。", "SceneManager::_transition_proc");
				_transition_to_error_scene();
				return;
			}
			//新しいシーンを初期化
			if (new_scene->Initialize() == false) {
				debug::SystemLog::instance().LogError(std::string("シーン遷移に失敗しました。新しいシーン(") + _next_scene_id + ")の初期化に失敗しました。", "SceneManager::_transition_proc");
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
			debug::SystemLog::instance().LogMessage(std::string("シーン(") + _next_scene_id + ")に遷移しました。", "SceneManager::_transition_proc");
			_next_scene_id = "";
		}

		bool SceneManager::Initialize()
		{
			//空のシーンをセット
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<EmptySceneDefinition>();
			std::shared_ptr<Scene> es = Scene::MakeShared(_game);
			SceneSystemSetUpper sssu; //シーンのシステムセットアッパー(特殊プロセスの設定)
			sssu(*es);
			ecd->SetUpScene(SceneAccessorForSetUp(*es), utility::ParameterHolder());
			es->Initialize();
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
			auto it = _scene_setupper_creaters.find(scene_name);
			if (it == _scene_setupper_creaters.end()) { return nullptr; }
			else {
				return (it->second)();
			}
		}

		void SceneManager::_transition_to_error_scene()
		{
			debug::SystemLog::instance().LogError("エラーシーンに遷移します。", "SceneManager::_transition_to_error_scene");
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<ErrorSceneDefinition>();
			std::shared_ptr<Scene> es = Scene::MakeShared(_game);
			SceneSystemSetUpper sssu; //シーンのシステムセットアッパー(特殊プロセスの設定)
			sssu(*es);
			ecd->SetUpScene(SceneAccessorForSetUp(*es),utility::ParameterHolder());
			es->Initialize();
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			_request = _Request::None;
			_is_next_scene_loaded = false;
		}

		utility::ParameterHolder SceneManager::_end_current_scene()
		{
			//現在のシーンを終了(現在のシーンがなかったら空のパラメータを格納)
			if (_current_scene) {
				utility::ParameterHolder next_scene_initialize_parameters = _current_scene_setupper->FinalizeScene(SceneAccessorForSetUp(*_current_scene), _next_scene_id, _transition_parameters);
				_current_scene->Finalize();
				return next_scene_initialize_parameters;
			}
			else { return utility::ParameterHolder(); }
		}

	}
}