#pragma once

#include <memory>

namespace planeta_engine{
	namespace utility {
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

			//ポインタへのアクセス
			T& operator->() {
				return *ptr_;
			}
			//ポインタへのアクセス
			const T& operator->() const {
				return *ptr_;
			}
			//参照へのアクセス
			T& operator*() {
				return *ptr_;
			}
			//参照へのアクセス
			const T& operator*() const {
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