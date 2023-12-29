#pragma once

#include <string>

#include "ParameterHolder.hpp"

namespace plnt {
	/*! シーンマネージャの公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_scene_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_scene_manager() = 0 { }

		/*! 次シーンを読み込む(遷移はしない)*/
		virtual bool load_next_scene(const std::string &scene_id) = 0;
		/*! 読み込み進行度取得(0.0-1.0。未読み込み時には0.0)*/
		[[nodiscard]]  virtual double get_load_progress() const = 0;
		/*! 読み込み中か*/
		[[nodiscard]] virtual bool is_loading() const = 0;
		/*! 遷移中か*/
		[[nodiscard]] virtual bool is_transitioning() const = 0;
		/*! 遷移可能か*/
		[[nodiscard]] virtual bool is_transitionable() const = 0;
		/*! 次のシーンIDを取得*/
		[[nodiscard]] virtual const std::string get_next_scene_id() const = 0;
		/*! 現在のシーンIDを取得*/
		[[nodiscard]] virtual const std::string get_current_scene_id() const = 0;
		/*! シーンの遷移(読み込みが必要。読み込み中だったら読み込みつつ遷移)*/
		virtual bool transition_scene(const util::ParameterHolder &transition_parameters) = 0;
		/*! シーンを読み込んで遷移する*/
		virtual bool load_and_transition_scene(const std::string &scene_id,
		                                    const util::ParameterHolder &transition_parameters =
			                                    util::ParameterHolder()) = 0;
		/*! 終了*/
		virtual void quit_scene() = 0;
		/*! エラーを発生させる*/
		virtual void trigger_error() = 0;
	};
}
