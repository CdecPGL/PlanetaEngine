#include "PlanetaEngine.h"

#include "StandardResourceManager.h"

namespace planeta {
	void Game::SetStandardManagers() {
		using namespace private_;
		SetResourceManager(std::make_shared<StandardResourceManager>());
	}
}