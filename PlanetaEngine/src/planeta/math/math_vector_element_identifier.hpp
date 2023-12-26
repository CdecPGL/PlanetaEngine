#pragma once

namespace plnt::math::vei {
	template <typename T>
	class dummy_identifier {
	protected:
		static constexpr int minimum_vector_size = 0;

		static void set_element_array_pointer(T *ptr) { }
	};
}
