#pragma once

#include "../core/scene_setupper.hpp"

namespace plnt::private_ {
	/*シーン遷移エラー時に呼び出されるシーン*/
	class s_error final : public scene_setupper {
	public:
		bool setup_scene(scene_setup_proxy &scene_proxy,
		                 const util::parameter_holder &initialize_parameters) override;

		util::parameter_holder finalize_scene([[maybe_unused]] scene_setup_proxy &scene,
		                                      [[maybe_unused]] const std::string &next_scene_id,
		                                      [[maybe_unused]] const util::parameter_holder &
		                                      finalize_parameters) override { return {}; }
	};
}
