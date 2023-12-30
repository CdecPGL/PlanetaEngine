#pragma once

#include "..\core\game_object_system_component.hpp"
#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class CTransform2D;

	/*! 二次元カメラコンポーネント*/
	class CCamera2D final : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CCamera2D);

	public:
		using super = private_::game_object_system_component;

		CCamera2D();
		~CCamera2D();
		/*! 位置を取得*/
		const plnt::Vector2Dd &position() const;
		/*! 位置を設定*/
		void position(const plnt::Vector2Dd &p);
		/*! 回転度を取得*/
		double rotation_rad() const;
		/*! 回転度を設定*/
		void rotation_rad(double r);
		/*! 拡大度を取得*/
		double expansion() const;
		/*! 拡大度を設定*/
		void expansion(double e);

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		double expansion_ = 1.0;

		non_owing_pointer<CTransform2D> transform2d_;
		virtual bool get_other_components_proc(const go_component_getter &com_getter) override;
		virtual void on_initialized() override;
		virtual void on_activated() override;
		virtual void on_inactivated() override;
		virtual void on_finalized() noexcept override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CCamera2D);
}
