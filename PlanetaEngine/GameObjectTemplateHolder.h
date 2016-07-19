#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

namespace planeta {
	class GameObjectBase;
	namespace core {
		class GameObjectTemplateHolder {
		public:
			/*�V�����Q�[���I�u�W�F�N�g���擾����(file_id��empty�̏ꍇ�̓t�@�C���̓ǂݍ��݂��s��Ȃ�)*/
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc);
		private:
			std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<GameObjectBase>>> game_object_templates_;
			/*�Q�[���I�u�W�F�N�g�̍쐬*/
			std::shared_ptr<GameObjectBase> CreateGameObject_(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc);
		};
	}
}
