#include "GameObjectTemplateHolder.h"
#include "PrefixUtility.h"
#include "Reflection.h"
#include "GameObjectBase.h"
#include "ResourceManager.h"
#include "RJson.h"

namespace planeta {
	namespace core {
		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::GetNewGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc) {
			//�e���v���[�g
			std::shared_ptr<GameObjectBase> go_temp;
			//�K�v�Ȃ�e���v���[�g���쐬�o�^���A�g�p����e���v���[�g���Z�b�g����
			auto gtit = game_object_templates_.find(game_object_type_id);
			if (gtit == game_object_templates_.end()) {
				go_temp = CreateGameObject_(game_object_type_id, file_id, post_instantiate_proc);
				game_object_templates_.emplace(game_object_type_id, std::unordered_map<std::string, std::shared_ptr<GameObjectBase>>{ { file_id, go_temp } });
			} else {
				decltype(auto) fileid_temp_map = gtit->second;
				auto fit = fileid_temp_map.find(file_id);
				if (fit == fileid_temp_map.end()) {
					go_temp = CreateGameObject_(game_object_type_id, file_id, post_instantiate_proc);
					fileid_temp_map.emplace(file_id, go_temp);
				} else {
					go_temp = fit->second;
				}
			}
			//�e���v���[�g���N���[�����A�Ԃ�
			auto ngo = go_temp->Clone();
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::CreateGameObject_(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc) {
			auto obj_type_id = core::AddPrefix(game_object_type_id, core::ObjectCategory::GameObject);
			//����
			auto ngo = Reflection::CreateObjectByID<GameObjectBase>(obj_type_id);
			if (ngo == nullptr) {
				PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�e���v���[�g(GameObjectTypeID:\"", game_object_type_id, "\"�̍쐬�Ɏ��s���܂����B");
				return nullptr;
			}
			//�C���X�^���X��������
			if (!ngo->ProcessInstantiation()) {
				PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�e���v���[�g(GameObjectTypeID:\"", game_object_type_id, "\"�̃C���X�^���X���������Ɏ��s���܂����B");
			}
			//�C���X�^���X���㏈��
			post_instantiate_proc(*ngo);
			//�t�@�C�����w�肳��Ă��Ȃ��Ȃ�A�ǂݍ��݂͍s��Ȃ��B
			if (file_id.empty()) {
				return ngo;
			}
			//�t�@�C���ǂݍ���
			auto json_res = ResourceManager::instance().GetResource<RJson>(file_id);
			if (json_res == nullptr) {
				PE_LOG_ERROR("�Q�[���I�u�W�F�N�g��`���\�[�X\"", file_id, "\"�̓ǂݍ��݂Ɏ��s���܂����B");
				return nullptr;
			}
			try {
				if(!ngo->ProcessLoading(*json_res->GetRoot().GetWithException<JSONObject>())) {
					PE_LOG_ERROR("�Q�[���I�u�W�F�N�g�փt�@�C����`\"",file_id,"\"��ǂݍ��ނ��Ƃ��ł��܂���ł����B");
					return nullptr;
				}
			} catch (JSONTypeError& e) {
				PE_LOG_ERROR("�Q�[���I�u�W�F�N�g��`���\�[�X��JSON�t�@�C���́A���[�gValue��Object�ł���K�v������܂��B");
				return nullptr;
			}
			//����
			return ngo;
		}

	}
}