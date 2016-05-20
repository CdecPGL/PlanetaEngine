#pragma once

#include <memory>

namespace planeta_engine {
	namespace utility {
		/*std::enable_shared_from_this���g�����Ƃ𐄏��I*/
		template<class C>
		class SharedPointerInstance {
		public:
			static std::shared_ptr<C> MakeShared() {
				std::shared_ptr<C> ptr = std::make_shared<C>();
				ptr->_this_shared = ptr;
				return ptr;
			}
			template<typename T>
			static std::shared_ptr<C> MakeShared(T& p) {
				std::shared_ptr<C> ptr = std::make_shared<C>(p);
				ptr->_this_shared = ptr;
				return ptr;
			}
			/*�p���I�u�W�F�N�g���쐬����Ƃ��͂�����g��*/
			template<class C2>
			static std::shared_ptr<C2> MakeShared() {
				std::shared_ptr<C2> ptr = std::make_shared<C2>();
				ptr->_this_shared = ptr;
				return ptr;
			}
		protected:
			/*�����̃X�}�[�g�|�C���^���擾�B�e�N���X��SharedPointerInstance���p�������ꍇ�A�^�͐e�N���X�ɂȂ��Ă���̂Ŏ����̃N���X�̌^�ɃL���X�g���Ďg�p����B���̍ہA�L���X�g�ł��邱�Ƃ͕ۏ؂���Ă���̂ŁAstatic�L���X�g�ł����B*/
			std::shared_ptr<C> this_shared()const { return _this_shared.lock(); }
		private:
			std::weak_ptr<C> _this_shared;
		};
	}
}