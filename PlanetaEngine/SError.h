#pragma once
#include "SceneSetUpper.h"

namespace planeta_engine {
	namespace core {
		/*�V�[���J�ڃG���[���ɌĂяo�����V�[��*/
		class SError : public SceneSetUpper {
		public:
			virtual bool SetUpScene(SceneSetUpProxy& scene, const utility::ParameterHolder& initialize_parameters) override;

			virtual utility::ParameterHolder FinalizeScene(SceneSetUpProxy& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) override
			{
				return{};
			}

			virtual const std::vector<std::string> GetTransitionableSceneID() const override
			{
				return{};
			}

		};
	}
}
