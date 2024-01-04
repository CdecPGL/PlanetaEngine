#pragma once

#include "../core/game_object_standard_component.hpp"

namespace plnt {
	class c_effect final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_effect);

	public:
		using super = game_object_standard_component;
		c_effect();
		~c_effect() override;
		/*! エフェクトを再生する*/
		bool play() const;
		/*! エフェクトを停止する*/
		bool stop() const;
		/*! エフェクトリソースIDを設定する*/
		void resource_id(const std::string &resource_id) const;
		/*! エフェクトを再生中か確認*/
		[[nodiscard]] bool is_playing() const;
		/*! ループするかのフラグを設定*/
		void loop_flag(bool f);
		/*! ループするかのフラグを取得*/
		[[nodiscard]] bool loop_flag() const;
		/*! 自動再生するかどうかのフラグを設定*/
		void auto_play(bool f);
		/*! 自動再生するかどうかのフラグを取得*/
		[[nodiscard]] bool auto_play() const;
		/*! エフェクトの拡大率を設定*/
		void expansion(double e);
		/*! エフェクトの拡大率を取得*/
		[[nodiscard]] double expansion() const;

	protected:
		bool get_other_components_proc(const go_component_getter &) override;
		void on_initialized() override;
		void on_activated() override;
		void on_inactivated() override;
		void on_finalized() noexcept override;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_effect);
}
