#include <cassert>

#include"ResourceManager.h"
#include "FileSystemManager.h"
#include "RCsv.h"
#include "SystemLog.h"
#include "FileAccessor.h"
#include "SystemVariables.h"

namespace planeta {
	namespace core {

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags) {
			auto ret = std::move(_check_newuse_and_nouse_tag_groups(need_tags));
			size_t new_num(ret.first.size()), no_num(ret.second.size()); //�V�K�ǂݍ��ݐ��Ɩ��g�p�����L�^
			//�A�����[�h����ׂ��^�O�́A���g�p�^�O�O���[�v���X�g�ɓo�^
			for (const std::string& tag : ret.second) {
				_AddUnusedtagGroups(tag);
			}
			auto load_tags = std::move(ret.first);
			//�V�K�ǂݍ���
			for (const auto& tag : load_tags) {
				_load_tag_group(tag);
			}
			PE_LOG_MESSAGE(new_num, "�̃^�O�O���[�v���V�K�ǂݍ��݂���A", no_num, "�̃^�O�����g�p�ƂȂ�܂����B");
			return true;
		}

		bool ResourceManager::UnloadUnusedResouces() {
			for (const auto& res_array : _unused_tag_map) {
				for (const auto& id : res_array.second) {
					id.second->Dispose();
				}
			}
			PE_LOG_MESSAGE(_unused_tag_map.size(), "�̖��g�p�^�O�O���[�v���A�����[�h����܂����B");
			_unused_tag_map.clear();
			return true;
		}

		bool ResourceManager::IsReady()const { return true; }

		double ResourceManager::GetPrepairProgress() const { return 1.0; }

		std::shared_ptr<ResourceBase> ResourceManager::_CreateResource(const std::string& attribute, const std::shared_ptr<const File>& file) {
			auto it = _resource_creator_map.find(attribute);
			if (it == _resource_creator_map.end()) { return nullptr; }
			return (it->second)(file);
		}

		bool ResourceManager::_load_tag_group(const std::string& tag) {
			auto using_map_it = _using_tag_id_map.find(tag);
			//���łɓǂݍ��܂�Ă���
			if (using_map_it != _using_tag_id_map.end()) { return true; } else { //�ǂݍ��܂�Ă��Ȃ�
				_using_tag_id_map.emplace(tag, std::vector<std::string>());
				using_map_it = _using_tag_id_map.find(tag);
			}
			//���g�p���X�g�ɑ��݂���
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it != _unused_tag_map.end()) {
				for (const auto& res : unused_map_it->second) {
					using_map_it->second.push_back(res.first); //�g�p�����\�[�X�^�O�}�b�v��ID��ǉ�
					_using_resources.insert(res); //�g�p�����\�[�XID�}�b�v�ɒǉ�
				}
				_unused_tag_map.erase(unused_map_it); //���g�p���X�g����폜
			} else { //�V�K�ǂݍ���
				auto tag_map_it = _tag_resouce_map.find(tag);
				if (tag_map_it == _tag_resouce_map.end()) {
					PE_LOG_ERROR("�v�����ꂽ�^�O�O���[�v�͑��݂��܂���B(", tag, ")");
					return false;
				}
				std::vector<std::string> ids;
				for (auto it = tag_map_it->second.begin(); it != tag_map_it->second.end(); ++it) {
					std::shared_ptr<const File> f = file_accessor_->LoadFile(it->file_name);
					if (f) {
						std::shared_ptr<ResourceBase> new_res = _CreateResource(it->type, f);
						if (new_res == nullptr) {
							PE_LOG_ERROR("���\�[�X�̍쐬�Ɏ��s���܂����B(�^�O�O���[�v=", tag, ",ID=", it->id, ",�t�@�C����=", it->file_name, ")");
						} else {
							_using_resources.emplace(it->id, new_res);
							ids.push_back(it->id);
						}
					} else {
						PE_LOG_ERROR("���\�[�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(�^�O�O���[�v\"", tag, "\"��\"", it->file_name, "\")");
					}
				}
				_using_tag_id_map.emplace(tag, std::move(ids));
			}
			return true;
		}

		bool ResourceManager::_LoadResourceList() {
			std::shared_ptr<const File> file = file_accessor_->LoadFile(_resource_list_file_name);
			if (file == nullptr) {
				PE_LOG_ERROR("���\�[�X���X�g(", _resource_list_file_name, ")�̓ǂݍ��݂Ɏ��s���܂����B");
				return false;
			}
			if (file->GetStatus() != File::FileStatus::Available) {
				PE_LOG_ERROR("���\�[�X���X�g�t�@�C����ǂݍ��߂܂���ł����B(", _resource_list_file_name, ")", _tag_resouce_map.size(), +"��)");
				return false;
			}
			auto csv = MakeResource<RCsv>();
			if (csv->Create(file) == false) {
				PE_LOG_ERROR("���\�[�X���X�g�t�@�C����CSV�`���Ƃ��ēǂݍ��߂܂���ł����B(", _resource_list_file_name, ")", _tag_resouce_map.size(), "��)");
				return false;
			}
			for (const auto& l : *csv) {
				ResourceData rd;
				std::vector<std::string> tags;
				if (l.size() < 4) {
					std::string line;
					for (auto it = l.begin(); it != l.end(); ++it) {
						line += *it;
						if (it != l.end() - 1) { line += ","; }
					}
					PE_LOG_WARNING("���\�[�X��`�̍��ڐ�������܂���B���̍s�̓X�L�b�v���܂��B(", line, ")");
					continue;
				}
				rd.id = l[0];
				rd.file_name = l[1];
				rd.type = l[2];
				for (unsigned int i = 3; i < l.size(); ++i) {
					auto it = _tag_resouce_map.find(l[i]);
					//�^�O�����o�ꂾ������ǉ����A���łɓo�ꂵ�Ă����炻����w���C�e���[�^����������
					if (it == _tag_resouce_map.end()) {
						_tag_resouce_map.emplace(l[i], std::vector<ResourceData>());
						it = _tag_resouce_map.find(l[i]);
					}
					it->second.push_back(rd);
				}
			}
			int total_resource_num = 0;
			for (const auto& tag_group : _tag_resouce_map) {
				total_resource_num += tag_group.second.size();
			}
			PE_LOG_MESSAGE("���\�[�X���X�g��ǂݍ��݂܂����B(", _tag_resouce_map.size(), "�̃^�O�O���[�v�A���v", total_resource_num, "�̃��\�[�X��`)");
			return true;
		}

		bool ResourceManager::Initialize() {
			assert(file_accessor_ != nullptr);
			if (_LoadResourceList() == false) {
				PE_LOG_ERROR("�������Ɏ��s���܂����B���\�[�X���X�g�̎擾�Ɏ��s���܂����B");
				return false;
			}
			return true;
		}

		void ResourceManager::Finalize() {
			_UnloadAllLoadedResources();
		}

		void ResourceManager::_UnloadAllLoadedResources() {
			for (auto& res : _using_resources) {
				res.second->Dispose();
			}
			for (auto& res : _unused_tag_map) {
				for (auto& p : res.second) {
					p.second->Dispose();
				}
			}
			_using_resources.clear();
			_using_tag_id_map.clear();
			_unused_tag_map.clear();
			PE_LOG_MESSAGE("���ׂẴ��\�[�X���A�����[�h���܂����B");
		}

		void ResourceManager::_AddUnusedtagGroups(const std::string& tag) {
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it == _unused_tag_map.end()) {
				_unused_tag_map.emplace(tag, std::vector<std::pair<std::string, std::shared_ptr<ResourceBase>>>());
				unused_map_it = _unused_tag_map.find(tag);
			} else { return; }
			auto it = _using_tag_id_map.find(tag);
			if (it != _using_tag_id_map.end()) {
				for (const auto& id : it->second) {
					auto rit = _using_resources.find(id);
					if (rit != _using_resources.end()) {
						unused_map_it->second.push_back(std::make_pair(id, rit->second));
						_using_resources.erase(rit);
					}
				}
				_using_tag_id_map.erase(it);
			}
		}

		std::shared_ptr<ResourceBase> ResourceManager::GetResource(const std::string& id) {
			auto it = _using_resources.find(id);
			if (it == _using_resources.end()) {
				PE_LOG_WARNING("�ǂݍ��܂�Ă��Ȃ����\�[�X���v������܂����B(ID:", id, ")");
				return nullptr;
			} else {
				return it->second;
			}
		}

		const std::unordered_set<std::string> ResourceManager::GetUsingTagGroups() const {
			std::unordered_set<std::string> out;
			for (const auto& t : _using_tag_id_map) {
				out.insert(t.first);
			}
			return std::move(out);
		}

		std::pair<std::vector<std::string>, std::vector<std::string>> ResourceManager::_check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags) {
			std::vector<std::string> new_use, no_use;
			std::unordered_set<std::string> need_tags_set;
			//�V���ɓǂݍ��ނׂ��^�O������(���łɖ��g�p�^�O�̌��������������邽�߁A�K�v�^�O�̘A�z�z�������Ă���)
			for (const auto& tag : need_tags) {
				if (_using_tag_id_map.find(tag) == _using_tag_id_map.end()) { new_use.push_back(tag); }
				need_tags_set.insert(tag);
			}
			//���g�p�ɂȂ�^�O������
			for (const auto& tagp : _using_tag_id_map) {
				if (need_tags_set.find(tagp.first) == need_tags_set.end()) { no_use.push_back(tagp.first); }
			}
			return std::make_pair(new_use, no_use);
		}

		void ResourceManager::SetFileAccessor_(const std::shared_ptr<FileAccessor>& f_scsr) {
			file_accessor_ = f_scsr;
		}

	}
}