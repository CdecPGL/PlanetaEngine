#include <windows.h>

#include "planeta/PlanetaEngine.hpp"

int WINAPI WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow) {
	using namespace plnt;
	auto& game = Game::instance();
	game.SetStandardManagers();

	if (!game.Initialize()) { return -1; }
	while(game.Update() == GameStatus::Continue){}
	game.Finalize();

	return 0;
}