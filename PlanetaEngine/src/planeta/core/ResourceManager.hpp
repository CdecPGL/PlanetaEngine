#pragma once

#include <set>
#include <vector>
#include <functional>
#include "planeta/core/IResourceManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	class FileManipulator;
	namespace private_ {
		/// <summary>���\�[�X�}�l�[�W��.</summary>
		class ResourceManager : public IResourceManager, public SubSystemManager {
		public:
			/// <summary>Destructor.</summary>
			virtual ~ResourceManager()override = default;
			/// <summary>������</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool Initialize() = 0;
			/// <summary>�I������</summary>
			virtual void Finalize() = 0;
			/// <summary>�A�����[�h�ΏۊO�̃^�O��ݒ�</summary>
			/// <param name="tags">The tags.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool SetNotUnloadTags(const std::set<std::string>& tags) = 0;
			/// <summary>�^�O�Ŏw�肳�ꂽ���\�[�X���܂Ƃ߂ēǂݍ���</summary>
			/// <param name="need_tag_groups">Groups the need tag belongs to.</param>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool PrepareResources(const std::vector<std::string>& need_tag_groups) = 0;
			/// <summary>���g�p�̃^���\�[�X���A�����[�h����</summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			virtual bool UnloadUnusedResouces() = 0;
			/// <summary>Resource�̏���������������</summary>
			/// <returns>True if ready, false if not.</returns>
			virtual bool IsReady()const = 0;
			/// <summary>�����i�s�x(�ǂݍ��݂��Ă��Ȃ�����1.0�Ƃ���)</summary>
			/// <returns>The prepair progress.</returns>
			virtual double GetPrepairProgress()const = 0;
			/// <summary>�t�@�C���A�N�Z�T���Z�b�g�B�������O�ɌĂяo��</summary>
			/// <param name="f_scsr">The scsr.</param>
			virtual void SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr) = 0;
			/// <summary>���\�[�X���X�g�t�@�C������ݒ�B�������O�ɌĂяo���K�v������</summary>
			/// <param name="file_name">Filename of the file.</param>
			virtual void SetResourceListFileName_(const std::string& file_name) = 0;
			/// <summary>���\�[�X�̑�����ǉ�</summary>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			template<class C>
			void AddResourceType(const std::string& type_name, const std::string& type_prefix) {
				OnResourceTypeAdded(typeid(C), type_name, type_prefix, []()->std::shared_ptr<ResourceBase> {
					return MakeResource<C>();
				});
			}
		protected:
			/// <summary>���\�[�X�N���G�[�^�֐��^</summary>
			using ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>()>;
		private:
			/// <summary>���\�[�X�pshared_ptr�쐬</summary>
			/// <returns>A std::shared_ptr&lt;Res&gt;</returns>
			template<class Res>
			static std::shared_ptr<Res> MakeResource() {
				static_assert(std::is_base_of<ResourceBase, Res>::value == true, "Res is not derived ResourceBase.");
				return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; }));
			}
			/// <summary>Executes the resource type added action.</summary>
			/// <param name="type">The type.</param>
			/// <param name="type_name">Name of the type.</param>
			/// <param name="type_prefix">The type prefix.</param>
			/// <param name="creator">The creator.</param>
			virtual void OnResourceTypeAdded(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator) = 0;
		};
	}
}
