#pragma once

#include <memory>

namespace planeta{
	namespace util {
		/*! @brief 所有しないポインタ
			
			参照カウントなどを全く行わない生ポインタのラッパークラス。
		*/
		template<typename T>
		class NonOwingPointer {
		public:
			//! 既定のコンストラクタ。nullptrがセットされる。
			NonOwingPointer() :ptr_(nullptr) {}
			//! 生ポインタからのコンストラクタ。暗黙的変換を許す。
			NonOwingPointer(T* ptr) :ptr_(ptr){};
			//! std::shared_ptrからのコンストラクタ。暗黙的変換を許す。
			NonOwingPointer(const std::shared_ptr<T>& s_ptr) :ptr_(s_ptr.get()) {};
			//! std::unique_ptrからのコンストラクタ。暗黙的変換を許す。
			NonOwingPointer(const std::unique_ptr<T>& u_ptr) :ptr_(u_ptr.get()) {};
			//! コピーコンストラクタ
			NonOwingPointer(const NonOwingPointer&) = default;
			//! 代入演算子
			NonOwingPointer& operator=(const NonOwingPointer&) = delete;
			//! nullptrとの等価比較演算子
			bool operator==(std::nullptr_t)const { return ptr_ == nullptr; }
			//! nullptrとの非等価比較演算子
			bool operator!=(std::nullptr_t)const { return !(ptr_ == nullptr); }
			//! 生ポインタへのアクセス
			T* operator->()const {
				return ptr_;
			}
			//! 参照へのアクセス
			T& operator*()const {
				return *ptr_;
			}
			//! ポインタがセットされているか
			operator bool()const {
				return ptr_ != nullptr;
			}
			//! ポインタのリセット
			void reset(NonOwingPointer no_ptr) {
				ptr_ = no_ptr.ptr_;
			}
		private:
			T* ptr_;
		};
	}
}