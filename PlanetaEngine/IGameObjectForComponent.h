#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"

namespace planeta {
	//�����Œ�`���ꂽ�֐��́AGameObjectBase���p�������N���X�ƁAGameObjectComponent�Ɍ��J�����B
	class IGameObjectForComponent : public IGameObject{
	public:
		//�R���|�[�l���g���^�Ŏ擾����B(GameObject�����ł�Component�ɔ񏊗L�|�C���^��p���ăA�N�Z�X���邽�߁AIGameObject�̓����֐����B��)
		template<class ComT>
		util::NonOwingPointer<ComT> GetComponent()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			return std::static_pointer_cast<ComT>(GetComponentByTypeInfo_(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
		}
		//�R���|�[�l���g���^�őS�Ď擾����B
		template<class ComT>
		std::vector<util::NonOwingPointer<ComT>> GetAllComponents()const {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value == true, "ComT must drive GameObjectComponent.");
			auto lst = std::move(GetAllComponentsByTypeInfo(typeid(ComT), [](GameObjectComponent* goc) {return dynamic_cast<ComT*>(goc) != nullptr; }));
			std::vector<util::NonOwingPointer<ComT>> out();
			for (auto&& com : lst) {
				out.push_back(std::static_pointer_cast<ComT>(com));
			}
			return std::move(lst);
		}
		//�Q�[���I�u�W�F�N�g���쐬
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& id) = 0;
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		virtual util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id) = 0;
	private:
		virtual std::vector<std::shared_ptr<GameObjectComponent>> GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker)const = 0;
	};
}