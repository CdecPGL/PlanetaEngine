#pragma once

#include <memory>
#include <unordered_map>
#include "WeakPointer.h"

namespace planeta_engine {
	class GameObjectComponent;
	namespace components {
		class GroundComponent;
		class TransformComponent;
	}
	namespace game {
		class IGameObjectManagerAccessor;
		class IGameProcessManagerAccessor;
		class IGameObjectAccessor {
		public:
			virtual ~IGameObjectAccessor() = 0 {};
			/*�Q�[���I�u�W�F�N�g��L����*/
			virtual bool Activate() = 0;
			/*�Q�[���I�u�W�F�N�g�𖳌���*/
			virtual bool InActivate() = 0;
			/*�Q�[���I�u�W�F�N�g���L����*/
			virtual bool is_active()const = 0;
			/*�Q�[���I�u�W�F�N�g��j��*/
			virtual void Dispose() = 0;
			/*�^�ŃR���|�[�l���g���擾(���z�e���v���[�g�֐��͒�`�ł��Ȃ����߂����Ŏ�������)*/
			template<class C>
			utility::WeakPointer<C> GetComponent()const {
				static_assert(std::is_base_of<GameObjectComponent, C>::value == true, "C is not derived Component.");
				const auto& cl = GetComponentList_();
				for (const auto& com : cl) {
					auto ptr = std::dynamic_pointer_cast<C>(com.second);
					if (ptr != nullptr) { return ptr; }
				}
				return nullptr;
			}
			/*ID�ŃR���|�[�l���g���擾*/
			virtual utility::WeakPointer<GameObjectComponent> GetComponent(int id) = 0;
			/*�����n�`�R���|�[�l���g���擾*/
			virtual utility::WeakPointer<components::GroundComponent> GetBelongingGroundComponent() = 0;
			/*�����n�`���擾(�Z�b�g����Ă��Ȃ��ꍇ�̓_�~�[���Ԃ����)*/
			virtual const components::GroundComponent& belonging_ground()const = 0;
			virtual components::GroundComponent& belonging_ground() = 0;
			/*�g�����X�t�H�[���R���|�[�l���g���擾*/
			virtual utility::WeakPointer<components::TransformComponent> GetTransformComponent() = 0;
			/*�g�����X�t�H�[�����擾*/
			virtual const components::TransformComponent& transform()const = 0;
			virtual components::TransformComponent& transform() = 0;
			/*�����n�`���Z�b�g*/
			virtual void SetBelongingGround(const utility::WeakPointer<components::GroundComponent>& belonging_ground) = 0;
			/*��Q�Ƃ��擾*/
			virtual utility::WeakPointer<IGameObjectAccessor> GetWeakPointer() = 0;
		private:
			virtual const std::unordered_map<int, std::shared_ptr<GameObjectComponent>>& GetComponentList_()const = 0;
		};
	}
}
