#pragma once
#include "SceneSetUpper.h"

namespace planeta_engine {
	namespace core {
		/*シーン遷移エラー時に呼び出されるシーン*/
		class ErrorSceneDefinition : public SceneSetUpper {
		public:
			virtual bool SetUpScene(ISceneSetup& scene, const utility::ParameterHolder& initialize_parameters) override
			{
				return true;
			}

			virtual utility::ParameterHolder FinalizeScene(ISceneSetup& scene, const std::string& next_scene_id, const utility::ParameterHolder& finalize_parameters) override
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
