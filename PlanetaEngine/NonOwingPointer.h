#pragma once

#include <memory>

namespace planeta{
	namespace util {
		//所有管理しないポインタ
		template<typename T>
		class NonOwingPointer {
		public:
			NonOwingPointer() :ptr_(nullptr) {}
			//各ポインタからのコンストラクタ。暗黙的変換を許す。
			NonOwingPointer(T* ptr) :ptr_(ptr){};
			NonOwingPointer(const std::shared_ptr<T>& s_ptr) :ptr_(s_ptr.get()) {};
			NonOwingPointer(const std::unique_ptr<T>& u_ptr) :ptr_(u_ptr.get()) {};

			NonOwingPointer(const NonOwingPointer&) = default;

			NonOwingPointer& operator=(const NonOwingPointer&) = delete;

			bool operator==(std::nullptr_t)const { return ptr_ == nullptr; }

			//ポインタへのアクセス
			T* operator->()const {
				return ptr_;
			}
			//参照へのアクセス
			T& operator*()const {
				return *ptr_;
			}
			//ポインタがセットされたか
			operator bool()const {
				return ptr_ != nullptr;
			}
			//ポインタのリセット
			void reset(NonOwingPointer no_ptr) {
				ptr_ = no_ptr.ptr_;
			}
		private:
			T* ptr_;
		};
	}
}