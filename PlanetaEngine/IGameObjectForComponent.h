#pragma once

#include "IGameObject.h"
#include "NonOwingPointer.h"

namespace planeta {
	/*! @brief GameObjectComponent��GameObject�̂��߂́A�Q�[���I�u�W�F�N�g�ւ̃A�N�Z�X��񋟂���C���^�[�t�F�C�X�N���X
	
		�����Œ�`���ꂽ�֐��́AGameObjectBase���p�������N���X�ƁAGameObjectComponent�Ɍ��J�����B
	*/
	class IGameObjectForComponent : public IGameObject{
	public:
		//! �Q�[���I�u�W�F�N�g���쐬
		virtual util::WeakPointer<IGameObject> CreateGameObject(const std::string& gameobject_type_id, const std::string& resource_id) = 0;
		//!�Q�[���I�u�W�F�N�g���쐬���ėL����
		virtual util::WeakPointer<IGameObject> CreateAndActivateGameObject(const std::string& gameobject_type_id, const std::string& resource_id) = 0;
		//! ��`�t�@�C����ǂݍ��܂Ȃ��ŃQ�[���I�u�W�F�N�g���쐬
		virtual util::WeakPointer<IGameObject> CreateDefaultGameObject(const std::string& gameobject_type_id) = 0;
		//! ��`�t�@�C����ǂݍ��܂Ȃ��ŃQ�[���I�u�W�F�N�g���쐬���ėL����
		virtual util::WeakPointer<IGameObject> CreateAndActivateDefaultGameObject(const std::string& gameobject_type_id) = 0;
	};
}