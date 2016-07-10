#include "InitFunctions.h"
#include "ResourceManager.h"
#include "SystemVariables.h"

#include "RGraph.h"
#include "RSound.h"
#include "RMusic.h"
#include "RCsv.h"
#include "RText.h"
#include "RXml.h"
#include "RJson.h"
#include "RFontDefinition.h"

namespace planeta {
	namespace core {
		namespace init_funcs {
			std::tuple<bool, std::function<void()>> InitializeResourceSystem(const std::shared_ptr<FileAccessor> res_file_accessor) {
				auto& rm = ResourceManager::instance();
				//�r���h�C�����\�[�X�̐ݒ�
				rm.AddResourceType<RGraph>(system_variables::resource::BuiltinGraphResourceTypeName); //�摜
				rm.AddResourceType<RSound>(system_variables::resource::BuiltinSoundResourceTypeName); //���ʉ�
				rm.AddResourceType<RMusic>(system_variables::resource::BuiltinMusicResourceTypeName); //���y
				rm.AddResourceType<RCsv>(system_variables::resource::BuiltinCSVResourceTypeName); //CSV�t�@�C��
				rm.AddResourceType<RText>(system_variables::resource::BuiltinTextResourceTypeName); //�e�L�X�g�t�@�C��
				rm.AddResourceType<RXml>(system_variables::resource::BuiltinXMLResourceTypeName); //XML�t�@�C��
				rm.AddResourceType<RFontDefinition>(system_variables::resource::BuiltinFontDefinitionResourceTypeName); //�t�H���g��`�t�@�C��
				rm.AddResourceType<RJson>(system_variables::resource::BuiltinJSONResourceTypeName); //JSON�t�@�C��
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