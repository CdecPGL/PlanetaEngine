#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <memory>
#include "Object.h"
#include "PointerSingletonTemplate.h"
#include "MakeResource.h"

namespace planeta_engine{
	namespace file_system {
		class FileAccessor;
		class File;
	}
	namespace core{
		class ResourceBase;
		/*Resource�Ǘ��N���X�B�񓯊��ǂݍ��݂͖����������A�������ɕύX���K�v�Ȃ��悤�ɔ񓯊��ǂݍ��݂Ɠ����悤�Ɉ����B
		PrepairResources�Ń��\�[�X�̏������J�n���AIsReady��true�ɂȂ�܂ő҂B������UnloadUnusedResoursces���ĂԁB
		����ł�IsReady�͏��true�AGetPrepairProgress�͏��1.0��Ԃ��B
		*/
		class ResourceManager : public utility::PointerSingletonTemplate<ResourceManager>{
			friend utility::PointerSingletonTemplate<ResourceManager>;
		public:
			bool Initialize()override;
			bool Finalize()override;
			/*Resource�̏����B�ǂݍ���ł��Ȃ��^�O�O���[�v��ǂݍ��݁A�ǂݍ��ݍς݂ł���Ȃ����̂�j�����X�g�ɓo�^����(param:�K�v�ȃ^�O���X�g)*/
			bool PrepareResources(const std::vector<std::string>& need_tag_groups);
			/*���g�p�̃^�O�O���[�v���A�����[�h����*/
			bool UnloadUnusedResouces();
			/*Resource�̏���������������*/
			bool IsReady()const;
			/*�����i�s�x(�ǂݍ��݂��Ă��Ȃ�����1.0�Ƃ���)*/
			double GetPrepairProgress()const;
			/*�g���Ă���^�O�O���[�v�����擾*/
			const std::unordered_set<std::string> GetUsingTagGroups()const;
			/*���\�[�X�̑�����ǉ�*/
			template<class C>
			void AddResourceType(const std::string& type_name) {
				_resource_creator_map.emplace(type_name, [](const std::shared_ptr<const file_system::File>& file)->std::shared_ptr<ResourceBase>{
					std::shared_ptr<ResourceBase> new_res = MakeResource<C>();
					return new_res->Create(file) ? new_res : nullptr;
				});
			}
			/*���\�[�X���X�g�t�@�C������ݒ�B�������O�ɌĂяo���K�v������*/
			void SetResourceListFileName(const std::string& file_name) { _resource_list_file_name = file_name; }
			/*���\�[�X���擾*/
			std::shared_ptr<ResourceBase> GetResource(const std::string& id);
		private:
			ResourceManager()=default;
			ResourceManager(const ResourceManager&) = delete;
			~ResourceManager() {};
			std::shared_ptr<file_system::FileAccessor> file_accessor_;
			/*���\�[�X���X�g�̃t�@�C����*/
			std::string _resource_list_file_name;

			struct ResourceData{
				/*ID*/
				std::string id = "\0";
				/*�t�@�C����*/
				std::string file_name = "\0";
				/*���*/
				std::string type = "\0";
			};
			/*�^�O�ɂ��Resource�f�[�^�}�b�v*/
			std::unordered_map<std::string, std::vector<ResourceData>> _tag_resouce_map;

			/*�g���Ă��郊�\�[�X�̃^�O�}�b�v(�^�O,ID�z��)*/
			std::unordered_map<std::string, std::vector<std::string>> _using_tag_id_map;
			/*�g���Ă��Ȃ����\�[�X�̃}�b�v<�^�O,<ID,���\�[�X>�z��>*/
			std::unordered_map<std::string,std::vector<std::pair<std::string,std::shared_ptr<ResourceBase>>>> _unused_tag_map;
			/*�g���Ă��郊�\�[�X(ID,���\�[�X)*/
			std::unordered_map<std::string, std::shared_ptr<ResourceBase>> _using_resources;

			/*�w��^�O�O���[�v�𖢎g�p�^�O�O���[�v�ɒǉ�*/
			void _AddUnusedtagGroups(const std::string& tag);
			/*�^�O�O���[�v��ǂݍ���*/
			bool _load_tag_group(const std::string& tag);
			/*�K�v�ȃ^�O�ƌ��ݓǂݍ��܂�Ă���^�O�O���[�v���r���A�V���Ɏg�����́A�����g��Ȃ����̂�Ԃ�*/
			std::pair<std::vector<std::string>, std::vector<std::string>> _check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags);


			/*���\�[�X�N���G�[�^�֐��^*/
			using _ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>(const std::shared_ptr<const file_system::File>&)>;
			/*Resource�̃^�C�v�ɂ��Resource�N���G�[�^�}�b�v*/
			std::unordered_map<std::string, _ResourceCreatorType> _resource_creator_map;
			/*���\�[�X�̍쐬*/
			std::shared_ptr<ResourceBase> _CreateResource(const std::string& type, const std::shared_ptr<const file_system::File>& file);

			/*���\�[�X���X�g�̓ǂݍ���*/
			bool _LoadResourceList();
			/*�ǂݍ��܂�Ă��邷�ׂẴ��\�[�X���A�����[�h����*/
			void _UnloadAllLoadedResources();
		};
	}
}