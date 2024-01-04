#pragma once

#include "../core/game_object_system_component.hpp"

namespace plnt {
	class screen_drawer_gui;

	class c_draw_gui : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_gui);

	public:
		using super = game_object_system_component;
		c_draw_gui();
		~c_draw_gui() override;
		/*描画処理*/
		void draw(screen_drawer_gui &drawer);
		/*! @brief 描画優先度を取得

		値が大きいほど手前に表示される*/
		[[nodiscard]] int draw_priority() const;
		/*! @brief 描画優先度を設定

		値が大きいほど手前に表示される*/
		c_draw_gui &draw_priority(int priority);

	protected:
		void on_initialized() override;
		void on_finalized() noexcept override;
		void on_activated() override;
		void on_inactivated() override;

	private:
		class impl;
		std::unique_ptr<impl> impl_;

		virtual void draw_proc(screen_drawer_gui &drawer) = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_gui);
}
