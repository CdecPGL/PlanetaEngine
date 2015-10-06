#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "GameObjectSetUpper.h"
#include "GameObjectSetUpProxy.h"
#include "PlanetComponent.h"
#include "GameObjectManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace planeta_engine::game;
	class TestGOSetter : public GameObjectSetUpper{
	private:
		virtual bool _SetUpGameObject(GameObjectSetUpProxy& gosup) override
		{
			gosup.AddComponent<components::PlanetComponent>();
			return true;
		}

	};
	TEST_CLASS(GameObjectTest)
	{
	public:
		GameObjectTest() {
			gom = GameObjectManager::MakeShared();
		}
		TEST_METHOD(TestTest)
		{
			auto go = gom->CreateGameObject(TestGOSetter());
		}
	private:
		std::shared_ptr<GameObjectManager> gom;
	};
}