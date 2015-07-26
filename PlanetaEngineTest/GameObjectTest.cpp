#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "GameObjectSetUpper.h"
#include "GameObjectSetUpProxy.h"
#include "PlanetComponent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlanetaEngineTest
{
	using namespace PlanetaEngine::Game;
	class TestGOSetter : public GameObjectSetUpper{
	public:
		bool operator()(GameObjectSetUpProxy& gosup){ 
			gosup.AddComponent<Components::PlanetComponent>();
			return true;
		}
	};
	TEST_CLASS(GameObjectTest)
	{
	public:
		
		TEST_METHOD(TestTest)
		{
			GameObjectPtr go = GameObject::Create(TestGOSetter());
		}

	};
}