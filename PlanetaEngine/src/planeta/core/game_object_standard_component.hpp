#pragma once

#include "game_object_component.hpp"

namespace plnt {
	class game_object_standard_component : public game_object_component {
	public:
		using super = game_object_component;

		game_object_standard_component() = default;
		game_object_standard_component(const game_object_standard_component &) = delete;
		game_object_standard_component(game_object_standard_component &&) = delete;
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition, modernize-use-equals-default)
		~game_object_standard_component() override = 0 {}
		game_object_standard_component &operator=(const game_object_standard_component &) = delete;
		game_object_standard_component &operator=(game_object_standard_component &&) = delete;

	private:
		/*シーンデータのセットは行わない。子クラスにはオーバーライド不可にしておく。*/
		void set_scene_internal_interface(const WeakPointer<private_::i_scene_internal> &) final { }
	};

	PE_REFLECTABLE_CLASS(game_object_standard_component);
}
