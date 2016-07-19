#pragma once

#include <memory>

namespace planeta{
	namespace util {
		/*! @brief ���L���Ȃ��|�C���^
			
			�Q�ƃJ�E���g�Ȃǂ�S���s��Ȃ����|�C���^�̃��b�p�[�N���X�B
		*/
		template<typename T>
		class NonOwingPointer {
		public:
			//! ����̃R���X�g���N�^�Bnullptr���Z�b�g�����B
			NonOwingPointer() :ptr_(nullptr) {}
			//! ���|�C���^����̃R���X�g���N�^�B�ÖٓI�ϊ��������B
			NonOwingPointer(T* ptr) :ptr_(ptr){};
			//! std::shared_ptr����̃R���X�g���N�^�B�ÖٓI�ϊ��������B
			NonOwingPointer(const std::shared_ptr<T>& s_ptr) :ptr_(s_ptr.get()) {};
			//! std::unique_ptr����̃R���X�g���N�^�B�ÖٓI�ϊ��������B
			NonOwingPointer(const std::unique_ptr<T>& u_ptr) :ptr_(u_ptr.get()) {};
			//! �R�s�[�R���X�g���N�^
			NonOwingPointer(const NonOwingPointer&) = default;
			//! ������Z�q
			NonOwingPointer& operator=(const NonOwingPointer&) = delete;
			//! nullptr�Ƃ̓�����r���Z�q
			bool operator==(std::nullptr_t)const { return ptr_ == nullptr; }
			//! nullptr�Ƃ̔񓙉���r���Z�q
			bool operator!=(std::nullptr_t)const { return !(ptr_ == nullptr); }
			//! ���|�C���^�ւ̃A�N�Z�X
			T* operator->()const {
				return ptr_;
			}
			//! �Q�Ƃւ̃A�N�Z�X
			T& operator*()const {
				return *ptr_;
			}
			//! �|�C���^���Z�b�g����Ă��邩
			operator bool()const {
				return ptr_ != nullptr;
			}
			//! �|�C���^�̃��Z�b�g
			void reset(NonOwingPointer no_ptr) {
				ptr_ = no_ptr.ptr_;
			}
		private:
			T* ptr_;
		};
	}
}