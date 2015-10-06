#pragma once
#include "Object.h"

namespace planeta_engine {
	namespace utility {
		/*ポインターでインスタンスを保持するシングルトン*/
		template<class C>
		class PointerSingletonTemplate : public core::Object {
		public:
			static bool Instantiate() {
				_CreateInstance();
				return _instance ? true : false;
			}
			static void Dispose() {
				_DisposeInstance();
			}
			virtual bool Initialize() = 0;
			virtual bool Finalize() = 0;
			static C& instance() { return *_instance; }
			static bool IsInstantiated() { return _instance ? true : false; }
		protected:
			PointerSingletonTemplate() = default;
			virtual ~PointerSingletonTemplate() = default;
		private:
			PointerSingletonTemplate(const PointerSingletonTemplate<C>&) = delete;
			PointerSingletonTemplate(PointerSingletonTemplate<C>&&) = delete;
			PointerSingletonTemplate& operator=(const PointerSingletonTemplate<C>&) = delete;
			PointerSingletonTemplate& operator=(PointerSingletonTemplate<C>&&) = delete;
			static C* _instance;
			static void _CreateInstance() {
				if (_instance == nullptr) { _instance = new C(); }
			}
			static void _DisposeInstance(){
				if (_instance) { 
					delete _instance;
					_instance = nullptr;
				}
			}
		};
		template<class C>
		C* PointerSingletonTemplate<C>::_instance = nullptr;
	}
}
