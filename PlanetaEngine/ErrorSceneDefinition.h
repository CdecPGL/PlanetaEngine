#pragma once
#include "SceneSetUpper.h"

namespace planeta_engine {
	namespace core {
		/*�V�[���J�ڃG���[���ɌĂяo�����V�[��*/
		class ErrorSceneDefinition : public SceneSetUpper {
		public:
			virtual bool SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) override
			{
				return true;
			}

			virtual utility::ParameterHolder FinalizeScene(SceneAccessorForSetUp& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) override
			{
				return utility::ParameterHolder();
			}

			virtual const std::vector<std::string> GetUseTagGroups() const override
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
