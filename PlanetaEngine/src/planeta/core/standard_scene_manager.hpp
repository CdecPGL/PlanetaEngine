#pragma once

#include <memory>

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
		class standard_scene_manager final : public scene_manager {
		public:
			explicit standard_scene_manager();

			//////////ユーザ関数//////////
			/*次シーンを読み込む(遷移はしない)*/
			bool load_next_scene(const std::string &scene_name) override;
			/*読み込み進行度取得(0.0-1.0。未読み込み時には0.0)*/
			[[nodiscard]] double get_load_progress() const override {
				return is_loading_ ? load_progress_ : is_next_scene_loaded_ ? 1.0f : 0.0f;
			}

			/*読み込み中か*/
			[[nodiscard]] bool is_loading() const override { return is_loading_; }
			/*遷移中か*/
			[[nodiscard]] bool is_transitioning() const override { return is_transitioning_; }
			/*遷移可能か*/
			[[nodiscard]] bool is_transitionable() const override {
				return (is_next_scene_loaded_ || is_loading_) && !is_transitioning_;
			}

			/*次のシーンIDを取得*/
			[[nodiscard]] const std::string get_next_scene_id() const override {
				return is_transitionable() ? next_scene_id_ : "";
			}

			/*現在のシーンIDを取得*/
			[[nodiscard]] const std::string get_current_scene_id() const override { return current_scene_id_; }
			/*シーンの遷移(読み込みが必要。読み込み中だったら読み込みつつ遷移)*/
			bool transition_scene(const util::parameter_holder &transition_parameters) override;
			/*シーンを読み込んで遷移する*/
			bool load_and_transition_scene(const std::string &scene_id,
			                               const util::parameter_holder &transition_parameters =
				                               util::parameter_holder()) override {
				return load_next_scene(scene_id) && transition_scene(transition_parameters);
			}

			/*終了*/
			void quit_scene() override { state_ = state::quit_requested; }
			/*エラー発生*/
			void trigger_error() override { state_ = state::error_occured; }

			//////////システム関数//////////
			/*マネージャの初期化*/
			bool initialize() override;
			/*マネージャの終了処理*/
			bool finalize() override;
			/*シーン処理*/
			scene_status process() override;
			/*リソースマネージャをセット(初期化前)*/
			void set_resource_manager(const std::shared_ptr<resource_manager> &mgr) override;

			void debug_information_add_handler(i_debug_information_adder &di_adder) override;

		private:
			std::shared_ptr<resource_manager> resource_manager_;
			/*現在のシーン*/
			std::shared_ptr<scene> current_scene_;
			/*次のシーン*/
			std::shared_ptr<scene> next_scene_;
			/*シーン設定クラス作成*/
			[[nodiscard]] static std::shared_ptr<scene_setupper> create_scene_set_upper(const std::string &scene_name);
			/*現在のシーンセットクラス*/
			std::shared_ptr<scene_setupper> current_scene_setupper_;
			/*次のシーンセットクラス*/
			std::shared_ptr<scene_setupper> next_scene_setupper_;

			/*状態*/
			enum class state { none, progress, transition_requested, quit_requested, error_occured };

			state state_;
			/*次のシーンID*/
			std::string next_scene_id_;
			/*次のシーンの初期化パラメータリスト*/
			util::parameter_holder transition_parameters_;
			/*現在のシーンID*/
			std::string current_scene_id_;
			/*遷移中か*/
			bool is_transitioning_;
			/*読み込み中か*/
			bool is_loading_;
			/*次のシーンが読み込まれているか*/
			bool is_next_scene_loaded_;
			/*読み込み進行度*/
			double load_progress_;

			/*遷移処理*/
			void transition_proc();
			/*現在のシーンを終了*/
			util::parameter_holder end_current_scene() const;
			/*エラーシーンへ遷移*/
			void transition_to_error_scene();
			/*シーンの初期化*/
			static bool initialize_scene(scene &scene, scene_setupper &setupper,
			                             const util::parameter_holder &init_param);
			/*シーンの終了処理*/
			static util::parameter_holder finalize_scene(scene &scene, scene_setupper &setupper,
			                                             const std::string &next_scene_id,
			                                             const util::parameter_holder &finalize_parameters);
		};
	}
}
