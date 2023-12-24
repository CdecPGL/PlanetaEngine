#pragma once

#include "planeta/PlanetaEngine.hpp"

class SDebugMode : public plnt::SceneSetUpper {
public:
	using Super = plnt::SceneSetUpper;
	virtual bool SetUpScene(plnt::SceneSetUpProxy& scene, const plnt::util::ParameterHolder& initialize_parameters) override;

	virtual plnt::util::ParameterHolder FinalizeScene(plnt::SceneSetUpProxy& scene, const std::string& next_scene_id, const plnt::util::ParameterHolder& finalize_parameters) override;

};
PE_SCENE_CLASS(SDebugMode);
