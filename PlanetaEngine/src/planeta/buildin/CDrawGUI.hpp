#pragma once

#include "..\core\game_object_system_component.hpp"

namespace plnt {
	class screen_drawer_gui;

	class CDrawGUI : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGUI);

	public:
		using super = private_::game_object_system_component;
		CDrawGUI();
		virtual ~CDrawGUI();
		/*描画処理*/
		void Draw(screen_drawer_gui &drawer);
		/*! @brief 描画優先度を取得

		値が大きいほど手前に表示される*/
		int draw_priority() const;
		/*! @brief 描画優先度を設定

		値が大きいほど手前に表示される*/
		CDrawGUI &draw_priority(int priority);

	protected:
		virtual void on_initialized() override;
		virtual void on_finalized() noexcept override;
		virtual void on_activated() override;
		virtual void on_inactivated() override;

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		virtual void DrawProc(screen_drawer_gui &drawer) = 0;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGUI);
}
