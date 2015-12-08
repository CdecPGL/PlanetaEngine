#pragma once

namespace planeta_engine {
	namespace math {
		namespace vei {
			template<typename T>
			class DumyIdentifer {
			protected:
				static constexpr int minimum_vector_size = 0;
				void SetElementArrayPointer(T* ptr) {}
			};
		}
	}
}
