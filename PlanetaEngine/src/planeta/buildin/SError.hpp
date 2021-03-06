﻿#pragma once

#include "planeta/core/SceneSetUpper.hpp"

namespace plnt {
	namespace private_ {
		/*シーン遷移エラー時に呼び出されるシーン*/
		class SError : public SceneSetUpper {
		public:
			virtual bool SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) override;

			virtual util::ParameterHolder FinalizeScene(SceneSetUpProxy& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) override
			{
				return{};
			}
		};
	}
}
