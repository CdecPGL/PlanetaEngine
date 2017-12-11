#pragma once

#include <set>
#include <vector>
#include <functional>
#include "IResourceManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class FileManipulator;
	namespace private_ {
		/*���\�[�X�}�l�[�W��*/
		class ResourceManager : public IResourceManager, public SubSystemManager {
		public:
			virtual ~ResourceManager()override = default;
			/*������*/
			virtual bool Initialize() = 0;
			/*�I������*/
			virtual void Finalize() = 0;
			/*�A�����[�h�ΏۊO�̃^�O��ݒ�*/
			virtual bool SetNotUnloadTags(const std::set<std::string>& tags) = 0;
			/*�^�O�Ŏw�肳�ꂽ���\�[�X���܂Ƃ߂ēǂݍ���*/
			virtual bool PrepareResources(const std::vector<std::string>& need_tag_groups) = 0;
			/*���g�p�̃^���\�[�X���A�����[�h����*/
			virtual bool UnloadUnusedResouces() = 0;
			/*Resource�̏���������������*/
			virtual bool IsReady()const = 0;
			/*�����i�s�x(�ǂݍ��݂��Ă��Ȃ�����1.0�Ƃ���)*/
			virtual double GetPrepairProgress()const = 0;
			/*�t�@�C���A�N�Z�T���Z�b�g�B�������O�ɌĂяo��*/
			virtual void SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) = 0;
			/*���\�[�X���X�g�t�@�C������ݒ�B�������O�ɌĂяo���K�v������*/
			virtual void SetResourceListFileName_(const std::string& file_name) = 0;
			/*���\�[�X�̑�����ǉ�*/
			template<class C>
			void AddResourceType(const std::string& type_name, const std::string& type_prefix) {
				AddResourceCreator(typeid(C), type_name, type_prefix, []()->std::shared_ptr<ResourceBase> {
					return MakeResource<C>();
				});
			}
		protected:
			/*���\�[�X�N���G�[�^�֐��^*/
			using ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>()>;
		private:
			/*���\�[�X�pshared_ptr�쐬*/
			template<class Res>
			static std::shared_ptr<Res> MakeResource() {
				static_assert(std::is_base_of<ResourceBase, Res>::value == true, "Res is not derived ResourceBase.");
				return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; }));
			}
			virtual void AddResourceTypeProc(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) = 0;
		};
	}
}
