#pragma once

namespace planeta_engine {
	namespace core {
		template <class C>
		class GetterSetterProcessor {
		public:
			GetterSetterProcessor(C& ins) :instance_(ins) {}

		private:
			C& instance_;

			template<typename T, T C::* MembderVariablePointer>
			const T& get()const {
				return instance_.*MembderVariablePointer;
			}
			template<typename T, T C::* MembderVariablePointer>
			const T& set()const {
				return instance_.*MembderVariablePointer;
			}
		};
	}
}