#pragma once

#include <typeinfo>

namespace plnt::reflection {
	namespace private_ {
		class TypeInfoWrapper final {
		public:
			TypeInfoWrapper() : target(nullptr) { }

			TypeInfoWrapper(const std::type_info &rhs) noexcept
				: target(&rhs) { }

			bool operator==(const TypeInfoWrapper &rhs) const noexcept { return *target == *rhs.target; }

			bool operator!=(const TypeInfoWrapper &rhs) const noexcept { return *target != *rhs.target; }

			bool operator<(const TypeInfoWrapper &rhs) const noexcept { return target->before(*rhs.target); }

			bool operator<=(const TypeInfoWrapper &rhs) const noexcept { return !target->before(*rhs.target); }

			bool operator>(const TypeInfoWrapper &rhs) const noexcept { return rhs.target->before(*target); }

			bool operator>=(const TypeInfoWrapper &rhs) const noexcept { return !rhs.target->before(*target); }

			std::size_t hash_code() const noexcept { return target->hash_code(); }

			const char *name() const noexcept { return target->name(); }

			const std::type_info &get_type_info() const { return *target; }

			struct hash {
				size_t operator()(::plnt::reflection::private_::TypeInfoWrapper ti) const noexcept {
					return ti.hash_code();
				}
			};

		private:
			const std::type_info *target;
		};
	}
}
