#include "ResourceBase.h"
#include "LogUtility.h"
#include "File.h"
#include "boost/filesystem/path.hpp"

namespace planeta {
	ResourceBase::~ResourceBase() {
		if (is_usable_) { PE_LOG_ERROR("���\�[�X�̉�����s���Ă��܂���B(", typeid(*this).name(), ")"); }
	}

	bool ResourceBase::Load(const File& file, const JsonFile& metadata, private_::ResourceManagerInternalAccessor& mgr_acsr) {
		if (is_usable_) {
			PE_LOG_ERROR("�ǂݍ��ݍς݂̃��\�[�X���t�@�C��\"", file.file_name(), "\"����ēǂݍ��݂��悤�Ƃ��܂����B���\�[�X�^�C�v��\"", typeid(*this).name(), "\"�B");
			return false;
		}
		if (!file.is_available()) {
			PE_LOG_ERROR("�����ȃt�@�C��\"", file.file_name(), "\"���w�肳��܂����B���\�[�X�^�C�v��\"", typeid(*this).name(), "\"�B");
			return false;
		}
		std::vector<std::shared_ptr<ResourceBase>> ref_list;
		//���̃��\�[�X�̑��݂���f�B���N�g���p�X�����߂�B
		std::string rpath = boost::filesystem::path(file.file_name()).parent_path().string();
		ResourceReferencer referencer{ mgr_acsr, rpath, ref_list };
		if (OnLoaded(file, metadata, referencer)) {
			reference_resources = std::move(ref_list);
			is_usable_ = true;
			return true;
		}
		else {
			PE_LOG_ERROR("�t�@�C��\"", file.file_name(), "\"����̃��\�[�X�쐬�Ɏ��s���܂����B���\�[�X�^�C�v��\"", typeid(*this).name(), "\"�B");
			return false;
		}
	}

	void ResourceBase::Dispose() {
		if (is_usable_) {
			OnDisposed();
			ClearReference();
			is_usable_ = false;
		} else {
			//�j�������̏d���͋���
			//PE_LOG_ERROR("�j���ς݂̃��\�[�X��j�����悤�Ƃ��܂����B���\�[�X�^�C�v��\"", typeid(*this).name(), "\"�B");
		}
	}

	size_t ResourceBase::reference_conunt() const {
		return reference_resources.size();
	}

	void ResourceBase::ClearReference() {
		reference_resources.clear();
	}
}