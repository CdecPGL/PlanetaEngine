#pragma once

#include "planeta/core/SceneSetUpper.hpp"

namespace plnt {
	namespace private_ {
		/*シーン遷移エラー時に呼び出されるシーン*/
		class SError : public scene_setupper {
		public:
			virtual bool setup_scene(scene_set_up_proxy &scene,
			                        const util::parameter_holder &initialize_parameters) override;

			virtual util::parameter_holder finalize_scene(scene_set_up_proxy &scene, const std::string &next_scene_id,
			                                            const util::parameter_holder &finalize_parameters) override {
				return {};
			}
		};
	}
}
