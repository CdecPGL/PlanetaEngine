#include "InitEnd.h"
#include "SystemVariables.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "SoundResource.h"
#include "MusicResource.h"
#include "CSVResource.h"
#include "TextResource.h"


namespace planeta_engine {
	namespace core{
		namespace init_end {
			bool SetUpResourceManager() {
				using namespace core;
				using namespace core;
				ResourceManager& rm = ResourceManager::instance();
				//�r���g�C�����\�[�X�̐ݒ�
				rm.AddResourceType<resources::GraphResource>(system_variables::BuiltinGraphResourceTypeName); //�摜
				rm.AddResourceType<resources::SoundResource>(system_variables::BuiltinSoundResourceTypeName); //���ʉ�
				rm.AddResourceType<resources::MusicResource>(system_variables::BuiltinMusicResourceTypeName); //���y
				rm.AddResourceType<resources::CSVResource>(system_variables::BuiltinCSVResourceTypeName); //CSV�t�@�C��
				rm.AddResourceType<resources::TextResource>(system_variables::BuiltinTextResourceTypeName); //�e�L�X�g�t�@�C��
				return true;
			}
		}
	}
}