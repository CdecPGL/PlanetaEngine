#pragma once

#include <memory>
#include <unordered_map>
#include<functional>
#include "Object.h"
#include "ParameterHolder.h"
#include "ISceneManagerAccessor.h"
#include "SharedPointerInstance.h"

namespace planeta_engine{
	namespace core {
		class KeyInputManager;
		class Scene;
		class SceneSetUpper;
		class IGameAccessor;
		/*シーン管理クラス
		非同期処理は未実装だが、実装した場合に変更が少なくなるよう、使い方は非同期処理がある場合と同じ。
		*/
		class SceneManager : public core::Object,public ISceneManagerAccessor{
		public:
			explicit SceneManager(IGameAccessor& engine);
			SceneManager(const SceneManager&) = delete;
			~SceneManager() = default;
			//////////ユーザ関数//////////
			/*次シーンを読み込む(遷移はしない)*/
			bool LoadNextScene(const std::string& scene_id)override;
			/*読み込み進行度取得(0.0-1.0。未読み込み時には0.0)*/
			double GetLoadProgress()const override { return _is_loading ? _load_progress : _is_next_scene_loaded ? 1.0f : 0.0f; }
			/*読み込み中か*/
			bool IsLoading()const override { return _is_loading; }
			/*遷移中か*/
			bool IsTransitioning()const override { return _is_transitioning; }
			/*遷移可能か*/
			bool IsTransitionable()const override { return (_is_next_scene_loaded || _is_loading) && !_is_transitioning; }
			/*次のシーンIDを取得*/
			const std::string GetNextSceneID()const override { return IsTransitionable() ? _next_scene_id : "\0"; }
			/*現在のシーンIDを取得*/
			const std::string GetCurrentSceneID()const override { return _current_scene_id; }
			/*シーンの遷移(読み込みが必要。読み込み中だったら読み込みつつ遷移)*/
			bool TransitionScene(const utility::ParameterHolder& transition_parameters)override;
			/*シーンを読み込んで遷移する*/
			bool LoadAndTransitionScene(const std::string& scene_id, const utility::ParameterHolder& transition_parameters = utility::ParameterHolder())override {
				return LoadNextScene(scene_id) && TransitionScene(transition_parameters);
			}

			//////////システム関数//////////
			/*マネージャの初期化*/
			bool Initialize();
			/*マネージャの終了処理*/
			bool Finalize();
			/*シーンの状況(継続、終了、エラー発生)*/
			enum class SceneStatus{ Continue, Quit, Error };
			/*シーン処理*/
			SceneStatus Process();

			/*シーンセッターを追加する(シーン定義クリエータを作成して、それを保持する)*/
			template<class C>
			void AddSceneSetUpper(const std::string& scene_name){
				static_assert(std::is_base_of<SceneSetUpper, C>::value == true, "C is not derived SceneSetUpper.");
				_scene_setupper_creaters.emplace(scene_name, []()->std::shared_ptr<SceneSetUpper> {return std::make_shared<C>(); });
			}
			/*シーンセッタークリエータを追加する(外部ファイルによるシーンセットアッパー作成に使用する予定)*/
			void AddSceneSetUpper(const std::string& scene_name,const std::function<std::shared_ptr<SceneSetUpper>()>& func) {
				_scene_setupper_creaters.emplace(scene_name, func);
			}
			/*終了*/
			void QuitScene()override { _request = _Request::Quit; }
			/*エラー発生*/
			void ErrorOccured()override { _request = _Request::Error; }
		private:
			/*シーン進行フラグ*/
			bool _scene_progress_flag;
			/*現在のシーン*/
			std::shared_ptr<Scene> _current_scene;
			/*次のシーン*/
			std::shared_ptr<Scene> _next_scene;
			/*シーン設定クラスクリエータ*/
			std::unordered_map<std::string, std::function<std::shared_ptr<SceneSetUpper>()>> _scene_setupper_creaters;
			/*シーン設定クラス作成*/
			std::shared_ptr<SceneSetUpper> _CreateSceneSetUpper(const std::string& scene_name);
			/*現在のシーンセットクラス*/
			std::shared_ptr<SceneSetUpper> _current_scene_setupper;
			/*次のシーンセットクラス*/
			std::shared_ptr<SceneSetUpper> _next_scene_setupper;
			/*リクエスト*/
			enum class _Request { None, Transition, Quit, Error };
			_Request _request;
			/*次のシーンID*/
			std::string _next_scene_id;
			/*次のシーンの初期化パラメータリスト*/
			utility::ParameterHolder _transition_parameters;
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
			/*エンジンへの参照*/
			IGameAccessor& _game;

			/*遷移処理*/
			void _transition_proc();
			/*現在のシーンを終了*/
			utility::ParameterHolder _end_current_scene();
			/*エラーシーンへ遷移*/
			void _transition_to_error_scene();

		};
	}
}