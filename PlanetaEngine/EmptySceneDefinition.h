#pragma once

#include "SceneSetUpper.h"

namespace planeta_engine {
	namespace core {
		/*シーンがセットされていない時に呼び出されるシーン*/
		class EmptySceneDefinition final: public SceneSetUpper {
		public:
			virtual bool SetUpScene(SceneSetUpProxy& scene, const utility::ParameterHolder& initialize_parameters) override;

			virtual utility::ParameterHolder FinalizeScene(SceneSetUpProxy& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) override {
				return utility::ParameterHolder();
			}

			virtual const std::vector<std::string> GetTransitionableSceneID() const override {
				return{};
			}

		};
	}
}