#pragma once

#include <memory>
#include <unordered_map>
#include<functional>
#include "scene_manager.hpp"
#include "parameter_holder.hpp"

namespace plnt {
	class scene_setupper;

	namespace private_ {
		class resource_manager;
		class scene;
		/*シーン管理クラス
		非同期処理は未実装だが、実装した場合に変更が少なくなるよう、使い方は非同期処理がある場合と同じ。
		*/
		class StandardSceneManager : public SceneManager {
		public:
			explicit StandardSceneManager();
			StandardSceneManager(const StandardSceneManager &) = delete;
			~StandardSceneManager() = default;
			//////////ユーザ関数//////////
			/*次シーンを読み込む(遷移はしない)*/
			bool load_next_scene(const std::string &scene_id) override;
			/*読み込み進行度取得(0.0-1.0。未読み込み時には0.0)*/
			double get_load_progress() const override {
				return _is_loading ? _load_progress : _is_next_scene_loaded ? 1.0f : 0.0f;
			}

			/*読み込み中か*/
			bool is_loading() const override { return _is_loading; }
			/*遷移中か*/
			bool is_transitioning() const override { return _is_transitioning; }
			/*遷移可能か*/
			bool is_transitionable() const override {
				return (_is_next_scene_loaded || _is_loading) && !_is_transitioning;
			}

			/*次のシーンIDを取得*/
			const std::string get_next_scene_id() const override { return is_transitionable() ? _next_scene_id : "\0"; }
			/*現在のシーンIDを取得*/
			const std::string get_current_scene_id() const override { return _current_scene_id; }
			/*シーンの遷移(読み込みが必要。読み込み中だったら読み込みつつ遷移)*/
			bool transition_scene(const util::parameter_holder &transition_parameters) override;
			/*シーンを読み込んで遷移する*/
			bool load_and_transition_scene(const std::string &scene_id,
			                            const util::parameter_holder &transition_parameters =
				                            util::parameter_holder()) override {
				return load_next_scene(scene_id) && transition_scene(transition_parameters);
			}

			/*終了*/
			void quit_scene() override { state_ = State::QuitRequested; }
			/*エラー発生*/
			void trigger_error() override { state_ = State::ErrorOccured; }

			//////////システム関数//////////
			/*マネージャの初期化*/
			bool Initialize();
			/*マネージャの終了処理*/
			bool Finalize();
			/*シーン処理*/
			SceneStatus_ Process_();
			/*リソースマネージャをセット(初期化前)*/
			void SetResouceManager(const std::shared_ptr<resource_manager> &mgr) override;

			void DebugInfotmationAddHandler(i_debug_information_adder &di_adder) override;

		private:
			std::shared_ptr<resource_manager> resource_manager_;
			/*現在のシーン*/
			std::shared_ptr<scene> _current_scene;
			/*次のシーン*/
			std::shared_ptr<scene> _next_scene;
			/*シーン設定クラス作成*/
			std::shared_ptr<scene_setupper> _CreateSceneSetUpper(const std::string &scene_name);
			/*現在のシーンセットクラス*/
			std::shared_ptr<scene_setupper> _current_scene_setupper;
			/*次のシーンセットクラス*/
			std::shared_ptr<scene_setupper> _next_scene_setupper;

			/*状態*/
			enum class State { None, Progress, TransitionRequested, QuitRequested, ErrorOccured };

			State state_;
			/*次のシーンID*/
			std::string _next_scene_id;
			/*次のシーンの初期化パラメータリスト*/
			util::parameter_holder _transition_parameters;
			/*現在のシーンID*/
			std::string _current_scene_id;
			/*遷移中か*/
			bool _is_transitioning;
			/*読み込み中か*/
			bool _is_loading;
			/*次のシーンが読み込まれているか*/
			bool _is_next_scene_loaded;
			/*読み込み進行度*/
			double _load_progress;

			/*遷移処理*/
			void _transition_proc();
			/*現在のシーンを終了*/
			util::parameter_holder _end_current_scene();
			/*エラーシーンへ遷移*/
			void _transition_to_error_scene();
			/*シーンの初期化*/
			bool InitializeScene_(scene &scene, scene_setupper &setupper, const util::parameter_holder &init_param);
			/*シーンの終了処理*/
			util::parameter_holder FinalizeScene_(private_::scene &scene, scene_setupper &setupper,
			                                     const std::string &next_scene_id,
			                                     const util::parameter_holder &finalize_parameters);
		};
	}
}
