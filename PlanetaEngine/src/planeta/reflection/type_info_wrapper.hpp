#pragma once

#include <typeinfo>

namespace plnt::reflection::private_ {
	class type_info_wrapper final {
	public:
		type_info_wrapper() : target_(nullptr) { }

		// ReSharper disable once CppNonExplicitConvertingConstructor
		type_info_wrapper(const std::type_info &rhs) noexcept
			: target_(&rhs) { }

		bool operator==(const type_info_wrapper &rhs) const noexcept { return *target_ == *rhs.target_; }

		bool operator!=(const type_info_wrapper &rhs) const noexcept { return *target_ != *rhs.target_; }

		bool operator<(const type_info_wrapper &rhs) const noexcept { return target_->before(*rhs.target_); }

		bool operator<=(const type_info_wrapper &rhs) const noexcept { return !target_->before(*rhs.target_); }

		bool operator>(const type_info_wrapper &rhs) const noexcept { return rhs.target_->before(*target_); }

		bool operator>=(const type_info_wrapper &rhs) const noexcept { return !rhs.target_->before(*target_); }

		[[nodiscard]] std::size_t hash_code() const noexcept { return target_->hash_code(); }

		[[nodiscard]] const char *name() const noexcept { return target_->name(); }

		[[nodiscard]] const std::type_info &get_type_info() const { return *target_; }

		struct hash {
			size_t operator()(const type_info_wrapper ti) const noexcept { return ti.hash_code(); }
		};

	private:
		const std::type_info *target_;
	};
}
