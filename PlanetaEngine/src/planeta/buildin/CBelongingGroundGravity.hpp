#pragma once

#include "..\core\game_object_standard_component.hpp"

namespace plnt {
	class CTransform2D;

	/*! 地面方向への重力発生コンポーネント*/
	class CBelongingGroundGravity final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CBelongingGroundGravity);

	public:
		using super = game_object_standard_component;
		/*! 重力スケールを取得*/
		const double gravity_scale() const { return _gravity_scale; }
		/*! 重力スケールを設定*/
		CBelongingGroundGravity &gravity_scale(double gs) {
			_gravity_scale = gs;
			return *this;
		}

	private:
		non_owing_pointer<CTransform2D> transform2d_;
		double _gravity_scale = 0.5;
		bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_initialized() override;
		void Update();
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CBelongingGroundGravity);
}
