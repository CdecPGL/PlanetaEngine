#pragma once

#include <functional>

namespace planeta {
	namespace util {
		class Coroutine {
		public:
			Coroutine();
//			Coroutine();
			void SetFunction();
			void Reset();
			void operator()();
			operator bool();
		private:

		};
	}
}