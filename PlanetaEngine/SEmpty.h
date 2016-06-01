#pragma once

#include "SceneSetUpper.h"

namespace planeta_engine {
	namespace core {
		/*�V�[�����Z�b�g����Ă��Ȃ����ɌĂяo�����V�[��*/
		class SEmpty final: public SceneSetUpper {
		public:
			virtual bool SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) override;

			virtual util::ParameterHolder FinalizeScene(SceneSetUpProxy& scene, const std::string& next_scene_id, const util::ParameterHolder& finalize_parameters) override {
				return{};
			}

			virtual const std::vector<std::string> GetTransitionableSceneID() const override {
				return{};
			}

		};
	}
}