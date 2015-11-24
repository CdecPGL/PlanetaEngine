#pragma once

namespace planeta_engine {
	namespace core {
		class Screen;
		class ScreenEffecter {
		public:
			virtual bool ApplyEffect(Screen& screen) = 0;
		private:
		};
	}
}