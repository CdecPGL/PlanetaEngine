#pragma once

#include "../core/game_object_system_component.hpp"
#include "../core/vector_2d.hpp"

namespace plnt {
	class c_transform_2d;

	/*! 二次元カメラコンポーネント*/
	class c_camera_2d final : public private_::game_object_system_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_camera_2d);

	public:
		using super = game_object_system_component;

		c_camera_2d();
		~c_camera_2d() override;
		/*! 位置を取得*/
		[[nodiscard]] const vector_2dd &position() const;
		/*! 位置を設定*/
		void position(const vector_2dd &p) const;
		/*! 回転度を取得*/
		[[nodiscard]] double rotation_rad() const;
		/*! 回転度を設定*/
		void rotation_rad(double r) const;
		/*! 拡大度を取得*/
		[[nodiscard]] double expansion() const;
		/*! 拡大度を設定*/
		void expansion(double e);

	private:
		class impl;
		std::unique_ptr<impl> impl_;

		double expansion_ = 1.0;

		weak_pointer<c_transform_2d> transform2d_;
		bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_initialized() override;
		void on_activated() override;
		void on_inactivated() override;
		void on_finalized() noexcept override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_camera_2d);
}
