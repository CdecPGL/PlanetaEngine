#include <windows.h>

#include "..\..\PlanetaEngine\src\planeta\planeta_engine.hpp"

int WINAPI WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                   [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow) {
	using namespace plnt;
	auto &game = game::instance();
	game.set_standard_managers();

	if (!game.initialize()) { return -1; }
	while (game.update() == game_status::play) {}
	game.finalize();

	return 0;
}
