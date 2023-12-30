#pragma once

#include <memory>

namespace plnt {
	/*! @brief 所有しないポインタ

		参照カウントなどを全く行わない生ポインタのラッパークラス。
	*/
	template <typename T>
	class non_owing_pointer {
	public:
		//! 既定のコンストラクタ。nullptrがセットされる。
		non_owing_pointer() = default;
		//! 生ポインタからのコンストラクタ。暗黙的変換を許す。
		// ReSharper disable once CppNonExplicitConvertingConstructor
		non_owing_pointer(T *ptr) : ptr_(ptr) { }
		//! std::shared_ptrからのコンストラクタ。暗黙的変換を許す。
		// ReSharper disable once CppNonExplicitConvertingConstructor
		non_owing_pointer(const std::shared_ptr<T> &s_ptr) : ptr_(s_ptr.get()) { }
		//! std::unique_ptrからのコンストラクタ。暗黙的変換を許す。
		// ReSharper disable once CppNonExplicitConvertingConstructor
		non_owing_pointer(const std::unique_ptr<T> &u_ptr) : ptr_(u_ptr.get()) { }
		//! コピーコンストラクタ
		non_owing_pointer(const non_owing_pointer &) = default;
		//! ムーブコンストラクタ
		non_owing_pointer(non_owing_pointer &&) = default;
		//! デストラクタ
		~non_owing_pointer() = default;
		//! 代入演算子
		non_owing_pointer &operator=(const non_owing_pointer &) = default;
		//! ムーブ演算子
		non_owing_pointer &operator=(non_owing_pointer &&) = default;
		//! nullptrとの等価比較演算子
		bool operator==(std::nullptr_t) const { return ptr_ == nullptr; }
		//! nullptrとの非等価比較演算子
		bool operator!=(std::nullptr_t) const { return !(ptr_ == nullptr); }

		//! 生ポインタへのアクセス
		T *operator->() const { return ptr_; }

		//! 参照へのアクセス
		T &operator*() const { return *ptr_; }

		//! ポインタがセットされているか。暗黙的変換を許す。
		// ReSharper disable once CppNonExplicitConversionOperator
		operator bool() const { return ptr_ != nullptr; }

		//! ポインタのリセット
		void reset(const non_owing_pointer &no_ptr) { ptr_ = no_ptr.ptr_; }

	private:
		T *ptr_;
	};
}
