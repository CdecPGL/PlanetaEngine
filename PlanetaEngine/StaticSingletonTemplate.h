#pragma once
#include "Object.h"

namespace planeta_engine {
	namespace utility {
		/*関数内静的変数でインスタンスを保持するシングルトン*/
		template<class C>
		class StaticSingletonTemplate : public core::Object {
		public:
			static C& GetInstance() {
				static C c;
				return c; 
			}
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
		protected:
			StaticSingletonTemplate() = default;
			virtual ~StaticSingletonTemplate() = default;
		private:
			StaticSingletonTemplate(const StaticSingletonTemplate<C>&) = delete;
			StaticSingletonTemplate(StaticSingletonTemplate<C>&&) = delete;
			StaticSingletonTemplate& operator=(const StaticSingletonTemplate<C>&) = delete;
			StaticSingletonTemplate& operator=(StaticSingletonTemplate<C>&&) = delete;
		};
	}
}

