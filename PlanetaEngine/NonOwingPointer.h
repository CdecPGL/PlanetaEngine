#pragma once

#include <memory>

namespace planeta_engine{
	namespace utility {
		//���L�Ǘ����Ȃ��|�C���^
		template<typename T>
		class NonOwingPointer {
		public:
			NonOwingPointer() :ptr_(nullptr) {}
			//�e�|�C���^����̃R���X�g���N�^�B�ÖٓI�ϊ��������B
			NonOwingPointer(NonOwingPointer* ptr) :ptr_(ptr){};
			NonOwingPointer(const std::shared_ptr<T>& s_ptr) :ptr_(s_ptr.get()) {};
			NonOwingPointer(const std::unique_ptr<T>& u_ptr) :ptr_(u_ptr.get()) {};

			NonOwingPointer(const NonOwingPointer&) = default;

			NonOwingPointer& operator=(const NonOwingPointer&) = default;

			//�|�C���^�ւ̃A�N�Z�X
			T& operator->() {
				return *ptr_;
			}
			//�|�C���^�ւ̃A�N�Z�X
			const T& operator->() const {
				return *ptr_;
			}
			//�|�C���^���Z�b�g���ꂽ��
			operator bool()const {
				return ptr_ != nullptr;
			}
		private:
			T* ptr_;
		};
	}
}