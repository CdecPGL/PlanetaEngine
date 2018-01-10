#pragma once

#include <functional>

namespace plnt {
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