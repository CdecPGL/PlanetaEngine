#include "stdafx.h"
#include "CppUnitTest.h"
#include "CPlanet.h"
#include "GameObjectManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace planeta;

	TEST_CLASS(GameObjectTest)
	{
	public:
		GameObjectTest() {
//			gom = GameObjectManager::MakeShared();
		}
		TEST_METHOD(TestTest)
		{
//			auto go = gom->CreateGameObject(TestGOSetter());
		}
	private:
		std::shared_ptr<private_::GameObjectManager> gom;
	};
}