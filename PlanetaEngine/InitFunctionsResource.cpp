#include "InitFunctions.h"
#include "ResourceManager.h"
#include "SystemVariables.h"

#include "GraphResource.h"
#include "SoundResource.h"
#include "MusicResource.h"
#include "CSVResource.h"
#include "TextResource.h"
#include "XMLResource.h"
#include "FontDefinitionResource.h"

namespace planeta_engine {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_accessor) {
				auto& rm = ResourceManager::instance();
				//�r���h�C�����\�[�X�̐ݒ�
				rm.AddResourceType<resources::GraphResource>(system_variables::resource::BuiltinGraphResourceTypeName); //�摜
				rm.AddResourceType<resources::SoundResource>(system_variables::resource::BuiltinSoundResourceTypeName); //���ʉ�
				rm.AddResourceType<resources::MusicResource>(system_variables::resource::BuiltinMusicResourceTypeName); //���y
				rm.AddResourceType<resources::CSVResource>(system_variables::resource::BuiltinCSVResourceTypeName); //CSV�t�@�C��
				rm.AddResourceType<resources::TextResource>(system_variables::resource::BuiltinTextResourceTypeName); //�e�L�X�g�t�@�C��
				rm.AddResourceType<resources::XMLResource>(system_variables::resource::BuiltinXMLResourceTypeName); //XML�t�@�C��
				rm.AddResourceType<resources::FontDefinitionResource>(system_variables::resource::BuiltinFontDefinitionResourceTypeName); //�t�H���g��`�t�@�C��
				rm.SetFileAccessor_(res_file_accessor); //�t�@�C���A�N�Z�T�Z�b�g
				rm.SetResourceListFileName_(core::system_variables::file_system::ResourceListFileName); //���\�[�X���X�g�t�@�C�����Z�b�g
				if (rm.Initialize()) {
					return{ true, [] {ResourceManager::instance().Finalize(); } };
				} else {
					PE_LOG_FATAL("���\�[�X�V�X�e���̏������Ɏ��s���܂����B");
					return{ false, [] {} };
				}
			}
		}
	}
}