#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"
#include "GameObjectComponentHolder.h"

namespace planeta_engine {
	//�����Œ�`���ꂽ�֐��́AGameObjectBase���p�������N���X�ƁAGameObjectComponent�Ɍ��J�����B
	class IGameObjectForComponent : public IGameObject{
	public:
		//�R���|�[�l���g���^�Ŏ擾����B
		template<class ComT>
		utility::NonOwingPointer<ComT> GetComponent() {
			return RefComponentHolder().GetComponent<ComT>();
		}
		//�R���|�[�l���g���^�őS�Ď擾����B
		template<class ComT>
		std::vector<utility::NonOwingPointer<ComT>> GetAllComponents() {
			auto lst = std::move(RefComponentHolder().GetAllComponents<ComT>());
			std::vector<utility::NonOwingPointer<ComT>> out();
			for (auto&& com : lst) {
				out.push_back(com);
			}
			return std::move(lst);
		}
		//�Q�[���I�u�W�F�N�g���쐬
		virtual utility::WeakPointer<IGameObject> CreateGameObject(const std::string& id) = 0;
		//�Q�[���I�u�W�F�N�g���쐬���ėL����
		virtual utility::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& id) = 0;
	private:
		virtual  GameObjectComponentHolder& RefComponentHolder() = 0;
	};
}