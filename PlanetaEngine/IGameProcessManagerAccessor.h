#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "boost/preprocessor.hpp"

/**
* @brief ����^�̗v�f��WeakPointer�Ŏ擾������J�������z�e���v���[�g�֐����^���I�Ɏ������邽�߂̃}�N���B
* ���J�������z�֐��Ƃ���"template<class C> utility::WeakPointer<C> func_name()const"���쐬�����B
* �q�N���X��"virtual utility::WeakPointer<element_base_type> func_name(std::function<bool(const std::shared_ptr<element_base_type>&)> type_checker)const = 0"��private�ŃI�[�o�[���C�h���Ȃ���΂Ȃ�Ȃ��B
* @param (func_name) ��������֐���
* @param (element_base_type) �v�f�̊��^
*/
#define PUBLIC_PURE_VIRTUAL_TEMPLATE_WEAKPOINTER_ELEMENT_GETTER_BY_TYPE(element_base_type,func_name)\
private:\
virtual utility::WeakPointer<element_base_type> func_name(std::function<bool(const std::shared_ptr<element_base_type>&)> type_checker)const = 0;\
public:\
template<class C>\
utility::WeakPointer<C> func_name()const {\
	return utility::static_weak_pointer_cast<C>(func_name(&utility::CheckSharedPointerTypeOfBase<C, element_base_type>));\
}

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	};
	namespace game {
		class GameProcess;
		class IGameProcessManagerAccessor {
		public:
			virtual ~IGameProcessManagerAccessor() = 0 {};
			/*�Q�[���v���Z�X���쐬����*/
		private:virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority) = 0;
		public:
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga, int id) {return std::make_shared<C>(ga, id); }, priority));
			}
			/*�Q�[���v���Z�X��ID����擾����*/
			virtual utility::WeakPointer<GameProcess> GetGameProcess(int id) = 0;
			/*�Q�[���v���Z�X��ID�ō폜����*/
			virtual bool RemoveGameProcess(int id) = 0;
			/*�V�X�e���v���Z�X���^����擾����*/
			PUBLIC_PURE_VIRTUAL_TEMPLATE_WEAKPOINTER_ELEMENT_GETTER_BY_TYPE(GameProcess, GetSystemProcess);
			/*�V�X�e���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) = 0;
		};
	}
}
