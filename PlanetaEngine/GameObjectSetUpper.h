#pragma once

namespace planeta_engine{
	namespace game{
		class GameObjectSetUpProxy;
		class GameObjectSetUpper{
		public:
			GameObjectSetUpper() = default;
			virtual ~GameObjectSetUpper() = default;
			bool operator()(GameObjectSetUpProxy& gosup) { return _SetUpDefaultComponent() && _SetUpGameObject(gosup); }
		private:
			bool _SetUpDefaultComponent();
			virtual bool _SetUpGameObject(GameObjectSetUpProxy& gosup) = 0;
		};
	}
}