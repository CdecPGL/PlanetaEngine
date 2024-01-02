#pragma once

#include "game_object_component.hpp"

namespace plnt::private_ {
	class game_object_system_component : public game_object_component {
	public:
		using super = game_object_component;
		/*シーンデータをセットして、子クラスが参照できるようにし、オーバーライドできないようにする。*/
		void set_scene_internal_interface(const weak_pointer<i_scene_internal> &scene_data) final {
			scene_internal_interface_ = scene_data;
		}

		game_object_system_component() = default;
		game_object_system_component(const game_object_system_component &) = delete;
		game_object_system_component(game_object_system_component &&) = delete;
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition, modernize-use-equals-default)
		~game_object_system_component() override = 0 {}
		game_object_system_component &operator=(const game_object_system_component &) = delete;
		game_object_system_component &operator=(game_object_system_component &&) = delete;

	protected:
		i_scene_internal &scene_internal_interface() { return *scene_internal_interface_; }
		const i_scene_internal &scene_internal_interface() const { return *scene_internal_interface_; }

	private:
		weak_pointer<i_scene_internal> scene_internal_interface_;
	};

	PE_REFLECTABLE_CLASS(game_object_system_component);
}
