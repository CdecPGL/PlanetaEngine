#pragma once

#include <memory>
#include <cassert>
#include <type_traits>

// ReSharper disable once CppUnusedIncludeDirective
#include "null_weak_pointer_exception.hpp"


namespace plnt {
	template <typename T>
	class weak_pointer;

	template <typename T0, typename T1>
	weak_pointer<T0> static_weak_pointer_cast(const weak_pointer<T1> &ptr);
	template <typename T0, typename T1>
	weak_pointer<T0> dynamic_weak_pointer_cast(const weak_pointer<T1> &ptr);

	template <typename T>
	class weak_pointer {
		template <typename T0, typename T1>
		friend weak_pointer<T0> static_weak_pointer_cast(const weak_pointer<T1> &ptr);
		template <typename T0, typename T1>
		friend weak_pointer<T0> dynamic_weak_pointer_cast(const weak_pointer<T1> &ptr);
		template <typename T0>
		friend class weak_pointer;

	public:
		weak_pointer() = default;
		weak_pointer(const weak_pointer &wp) = default;
		weak_pointer(weak_pointer &&wp) = default;

		// ReSharper disable once CppNonExplicitConvertingConstructor
		weak_pointer(std::nullptr_t) {}

		template <typename T1>
		// ReSharper disable once CppNonExplicitConvertingConstructor
		weak_pointer(const weak_pointer<T1> &wp) : w_ptr_(wp.w_ptr_) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
		}

		template <typename T1>
		// ReSharper disable once CppNonExplicitConvertingConstructor
		weak_pointer(weak_pointer<T1> &&wp) : w_ptr_(std::move(wp.w_ptr_)) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
		}

		template <typename T1>
		// ReSharper disable once CppNonExplicitConvertingConstructor
		weak_pointer(const std::weak_ptr<T1> &wp) : w_ptr_(wp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
		}

		template <typename T1>
		// ReSharper disable once CppNonExplicitConvertingConstructor
		weak_pointer(const std::shared_ptr<T1> &sp) : w_ptr_(sp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
		}

		~weak_pointer() = default;

		/*スマポを取得*/
		[[nodiscard]] std::shared_ptr<T> get_shared() const {
			auto s_ptr = w_ptr_.lock();
			#ifdef _DEBUG //NULL参照が発生した場合、Debugビルド時はassertし、
			assert(s_ptr != nullptr);
			#else //それ以外では例外を投げる
			if (s_ptr == nullptr) {
				throw null_weak_pointer_exception(std::string("Null weak pointer is called."));
			}
			#endif
			return s_ptr;
		}

		[[nodiscard]] std::weak_ptr<T> get_weak_ptr() const {
			#ifdef _DEBUG //NULL参照が発生した場合、Debugビルド時はassertし、
			assert(!w_ptr_.expired());
			#else //それ以外では例外を投げる
			if (w_ptr_.expired()) {
				throw null_weak_pointer_exception(std::string("Null weak pointer is called."));
			}
			#endif
			return w_ptr_;
		}

		/*ポインターを取得*/
		[[nodiscard]] T *get() const {
			#ifdef _DEBUG //NULL参照が発生した場合、Debugビルド時はassertし、
			assert(!w_ptr_.expired());
			#else //それ以外では例外を投げる
			if (w_ptr_.expired()) {
				throw null_weak_pointer_exception(std::string("Null weak pointer is called."));
			}
			#endif
			return w_ptr_._Get();
		}

		weak_pointer &operator=(const weak_pointer &wp) = default;
		weak_pointer &operator=(weak_pointer &&wp) = default;

		template <typename T1>
		weak_pointer &operator=(const weak_pointer<T1> &wp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
			w_ptr_ = wp.w_ptr_;
			return *this;
		}

		template <typename T1>
		weak_pointer &operator=(weak_pointer<T1> &&wp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
			w_ptr_ = std::move(wp.w_ptr_);
			return *this;
		}

		template <typename T1>
		weak_pointer &operator=(const std::weak_ptr<T1> &wp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
			w_ptr_ = wp;
			return *this;
		}

		template <typename T1>
		weak_pointer &operator=(const std::shared_ptr<T1> &sp) {
			static_assert(std::is_base_of_v<T, T1> == true, "T is not base of T1.");
			w_ptr_ = sp;
			return *this;
		}

		//////////アクセス演算子//////////
		T *operator->() const {
			std::shared_ptr<T> s_ptr = w_ptr_.lock();
			#ifdef _DEBUG //NULL参照が発生した場合、Debugビルド時はassertし、
			assert(s_ptr != nullptr);
			#else //それ以外では例外を投げる
			if (s_ptr == nullptr) {
				throw null_weak_pointer_exception(std::string("Null weak pointer is called."));
			}
			#endif
			return s_ptr.get();
		}

		//////////ポインタ参照演算子//////////
		T &operator*() const {
			std::shared_ptr<T> s_ptr = w_ptr_.lock();
			#ifdef _DEBUG //NULL参照が発生した場合、Debugビルド時はassertし、
			assert(s_ptr != nullptr);
			#else
			if (s_ptr == nullptr) {
				throw null_weak_pointer_exception(std::string("Null weak pointer is called."));
			}
			#endif //それ以外では例外を投げる
			return *s_ptr;
		}

		// ReSharper disable once CppNonExplicitConversionOperator
		operator bool() const { return !w_ptr_.expired(); }

		bool operator==(const bool b) const { return operator bool() == b; }

		bool operator!=(bool b) const { return !((*this) == b); }

		bool operator==(std::nullptr_t) const { return w_ptr_.expired(); }

		bool operator!=(std::nullptr_t) const { return !((*this) == nullptr); }

		using pointer_type = T;

	private:
		std::weak_ptr<T> w_ptr_;
	};

	template <typename T0, typename T1>
	weak_pointer<T0> static_weak_pointer_cast(const weak_pointer<T1> &ptr) {
		return weak_pointer<T0>(std::static_pointer_cast<T0>(ptr.w_ptr_.lock()));
	}

	template <typename T0, typename T1>
	weak_pointer<T0> dynamic_weak_pointer_cast(const weak_pointer<T1> &ptr) {
		return weak_pointer<T0>(std::dynamic_pointer_cast<T0>(ptr.w_ptr_.lock()));
	}
}
