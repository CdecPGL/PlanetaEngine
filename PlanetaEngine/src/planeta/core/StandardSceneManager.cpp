#include "ResourceManager.hpp"
#include "StandardSceneManager.hpp"
#include "i_debug_manager.hpp"
#include "Scene.hpp"
#include "SceneSetUpper.hpp"
#include "planeta/buildin/SError.hpp"
#include "log_utility.hpp"
#include "null_weak_pointer_exception.hpp"
#include "prefix_utility.hpp"
#include "StandardSceneManagerUtility.hpp"

namespace plnt {
	namespace private_ {
		StandardSceneManager::StandardSceneManager() : state_(State::None), _is_transitioning(false),
		                                               _is_loading(false), _is_next_scene_loaded(false),
		                                               _load_progress(0.0) { }


		SceneStatus_ StandardSceneManager::Process_() {
			try {
				switch (state_) {
					case State::None:
						PE_LOG_WARNING("シーンが開始されていません。");
						break;
					case State::TransitionRequested:
						_transition_proc();
					case State::Progress:
						_current_scene->Update();
						return SceneStatus_::Continue;
					case State::QuitRequested:
						return SceneStatus_::Quit;
					case State::ErrorOccured:
						return SceneStatus_::Error;
					default:
						break;
				}
				return SceneStatus_::Error;
			} catch (null_weak_pointer_exception &e) {
				PE_LOG_ERROR("Scene::Updateで無効なWeakPointerが参照されました。", e.what());
				return SceneStatus_::Error;
			}
		}

		bool StandardSceneManager::load_next_scene(const std::string &scene_name) {
			if (is_loading() || is_transitioning()) {
				PE_LOG_ERROR("シーンの読み込みに失敗しました。シーンの読み込み中、または遷移中のため新たにシーン(", scene_name, ")を読み込むことはできません。");
				return false;
			} //すでに読み込み中か、遷移中のため、あらたに読み込みできない
			//シーン定義クラス作成
			_next_scene_setupper = _CreateSceneSetUpper(scene_name);
			//シーン定義クラスが作成できなかった
			if (_next_scene_setupper == nullptr) {
				PE_LOG_ERROR("シーンの読み込みに失敗しました。指定されたシーン(", scene_name, ")のセットアッパーが存在しません。");
				return false;
			}
			//リソース読み込み
			//シーンIDと同じIDのタグをアンロード対象外に指定し、読み込む
			assert(resource_manager_ != nullptr);
			auto &rm = *resource_manager_;
			bool scc = rm.set_not_unload_tags({scene_name});
			scc &= rm.prepare_resources({scene_name});
			if (!scc) {
				PE_LOG_WARNING("指定されたシーン(", scene_name, ")のリソース準備に失敗しました。読み込みに失敗したか、対象のリソースが存在しない可能性があります。");
				//return false;
			} //リソースの準備ができなかった
			//準備完了
			_next_scene_id = scene_name;
			_is_next_scene_loaded = true;
			PE_LOG_MESSAGE("指定されたシーン(", scene_name, ")を読み込みました。");
			return true;
		}

		bool StandardSceneManager::transition_scene(const util::parameter_holder &transition_parameters) {
			if (is_transitionable() == false) {
				PE_LOG_ERROR("シーン遷移予約に失敗しました。遷移中のため、新たに遷移処理をはじめることはできません。");
				return false;
			} //遷移処理を始めることはできない
			//シーン遷移要求
			state_ = State::TransitionRequested;
			_transition_parameters = transition_parameters;
			PE_LOG_MESSAGE("シーン遷移予約を行いました。");
			return true;
		}

		void StandardSceneManager::_transition_proc() {
			//現在のシーンを終了
			util::parameter_holder next_scene_initialize_parameters = _end_current_scene();
			//新しいシーンを作成
			std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();
			//新しいシーンを初期化
			if (!InitializeScene_(*new_scene, *_next_scene_setupper, next_scene_initialize_parameters)) {
				PE_LOG_ERROR("シーン遷移に失敗しました。新しいシーン(", _next_scene_id, ")のセットアップまたは初期化に失敗しました。");
				_transition_to_error_scene();
				return;
			}
			_current_scene = std::move(new_scene);
			_current_scene_setupper = std::move(_next_scene_setupper);
			//未使用リソースを削除(シーンの更新前のため、ここで削除してよい)
			assert(resource_manager_ != nullptr);
			resource_manager_->unload_unused_resources();
			//リクエストと準備状況をリセット
			state_ = State::Progress;
			_is_next_scene_loaded = false;
			PE_LOG_MESSAGE("シーン(", _next_scene_id, ")に遷移しました。");
			_current_scene_id = _next_scene_id;
			_next_scene_id = "";
		}

		bool StandardSceneManager::Initialize() {
			_current_scene.reset();
			_current_scene_setupper.reset();
			state_ = State::None;
			_is_next_scene_loaded = false;
			CreateDebugInformationChannel("SceneManager");
			return true;
		}

		bool StandardSceneManager::Finalize() {
			DeleteDebugInformationChannel();
			//現在のシーンを終了
			_end_current_scene();
			state_ = State::None;
			return true;
		}

		std::shared_ptr<SceneSetUpper> StandardSceneManager::_CreateSceneSetUpper(const std::string &scene_name) {
			//シーン名にプレフィックスをつけたクラスを作成。
			auto setupper = reflection::reflection::create_object_by_object_type_id<SceneSetUpper>(
				private_::add_prefix(scene_name, private_::object_category::scene));
			return setupper;
		}

		void StandardSceneManager::_transition_to_error_scene() {
			PE_LOG_ERROR("エラーシーンに遷移します。");
			std::shared_ptr<SceneSetUpper> ecd = std::make_shared<SError>();
			std::shared_ptr<Scene> es = std::make_shared<Scene>();
			InitializeScene_(*es, *ecd, util::parameter_holder());
			_current_scene = std::move(es);
			_current_scene_setupper = ecd;
			state_ = State::Progress;
			_is_next_scene_loaded = false;
		}

		util::parameter_holder StandardSceneManager::_end_current_scene() {
			//現在のシーンを終了(現在のシーンがなかったら空のパラメータを格納)
			if (_current_scene) {
				return FinalizeScene_(*_current_scene, *_current_scene_setupper, _next_scene_id,
				                      _transition_parameters);
			} else { return util::parameter_holder(); }
		}

		void StandardSceneManager::SetResouceManager(const std::shared_ptr<resource_manager> &mgr) {
			resource_manager_ = mgr;
		}

		void StandardSceneManager::DebugInfotmationAddHandler(i_debug_information_adder &di_adder) {
			di_adder.add_line_v("現在のシーンID:", _current_scene_id);
			if (_current_scene) { _current_scene->DebugInformationAddHandle(di_adder); }
		}

		bool StandardSceneManager::InitializeScene_(Scene &scene, SceneSetUpper &setupper,
		                                            const util::parameter_holder &init_param) {
			//標準のシーンModuleをセット
			SetStandardSceneModules(scene);
			//シーンModuleにシーンを登録
			scene.SetSceneToModules();
			//システム設定(特殊プロセスの作成やシーンデータの更新)
			if (!StandardSystemSetUpScene(scene)) {
				PE_LOG_ERROR("シーンのシステム設定に失敗しました。");
				return false;
			}
			//初期化
			if (!scene.Initialize()) {
				PE_LOG_ERROR("シーンの初期化に失敗しました。");
				return false;
			}
			//固有設定
			if (!setupper.InitializeScene(scene, init_param)) {
				PE_LOG_ERROR("シーンの固有設定に失敗しました。");
				return false;
			}
			return true;
		}

		util::parameter_holder StandardSceneManager::FinalizeScene_(private_::Scene &scene, SceneSetUpper &setupper,
		                                                           const std::string &next_scene_id,
		                                                           const util::parameter_holder &finalize_parameters) {
			auto ret = setupper.TerminateScene(scene, next_scene_id, finalize_parameters); //固有終了処理
			scene.Finalize(); //終了処理
			return ret;
		}
	}
}
