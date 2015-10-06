#include"ResourceManager.h"
#include "boost/lexical_cast.hpp"
#include "FileLoadManager.h"
#include "CSVResource.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace core{

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags)
		{
			auto ret = std::move(_check_newuse_and_nouse_tag_groups(need_tags));
			size_t new_num(ret.first.size()), no_num(ret.second.size()); //�V�K�ǂݍ��ݐ��Ɩ��g�p�����L�^
			//�A�����[�h����ׂ��^�O�́A���g�p�^�O�O���[�v���X�g�ɓo�^
			for (const std::string& tag : ret.second) {
				_AddUnusedtagGroups(tag);
			}
			auto load_tags = std::move(ret.first);
			//�V�K�ǂݍ���
			for (const auto& tag : load_tags){
				_load_tag_group(tag);
			}
			debug::SystemLog::instance().LogMessage(boost::lexical_cast<std::string>(new_num) + "�̃^�O�O���[�v���V�K�ǂݍ��݂���A" + boost::lexical_cast<std::string>(no_num) + "�̃^�O�����g�p�ƂȂ�܂����B", "ResourceManager::PrepairResource");
			return true;
		}

		bool ResourceManager::UnloadUnusedResouces()
		{
			for (const auto& res_array : _unused_tag_map){
				for (const auto& id : res_array.second) {
					id.second->Dispose();
				}
			}
			debug::SystemLog::instance().LogMessage(boost::lexical_cast<std::string>(_unused_tag_map.size()) + "�̖��g�p�^�O�O���[�v���A�����[�h����܂����B", "ResourceManager::UnloadUnusedResources");
			_unused_tag_map.clear();
			return true;
		}

		bool ResourceManager::IsReady()const{ return true; }

		double ResourceManager::GetPrepairProgress() const { return 1.0; }

		std::shared_ptr<ResourceBase> ResourceManager::_CreateResource(const std::string& attribute,const std::shared_ptr<file_system::File>& file)
		{
			auto it = _resource_creator_map.find(attribute);
			if (it == _resource_creator_map.end()) { return nullptr; }
			return (it->second)(file);
		}

		bool ResourceManager::_load_tag_group(const std::string& tag)
		{
			debug::SystemLog& sl = debug::SystemLog::instance();
			auto using_map_it = _using_tag_id_map.find(tag);
			//���łɓǂݍ��܂�Ă���
			if (using_map_it != _using_tag_id_map.end()) { return true;}
			else { //�ǂݍ��܂�Ă��Ȃ�
				_using_tag_id_map.emplace(tag, std::vector<std::string>());
				auto using_map_it = _using_tag_id_map.find(tag);
			}
			//���g�p���X�g�ɑ��݂���
			auto it = _unused_tag_map.find(tag);
			if (it != _unused_tag_map.end()) {
				for (const auto& res : it->second) {
					using_map_it->second.push_back(res.first); //�g�p�����\�[�X�^�O�}�b�v��ID��ǉ�
					_using_resources.insert(res); //�g�p�����\�[�XID�}�b�v�ɒǉ�
				}
				_unused_tag_map.erase(it); //���g�p���X�g����폜
			}
			else { //�V�K�ǂݍ���
				using namespace file_system;
				auto tag_map_it = _tag_resouce_map.find(tag);
				if (tag_map_it == _tag_resouce_map.end()) {
					debug::SystemLog::instance().LogError(std::string("�v�����ꂽ�^�O�O���[�v�͑��݂��܂���B(") + tag + ")", "ResourceManager::_load_tag_group");
					return false;
				}
				std::vector<std::string> ids;
				for (auto it = tag_map_it->second.begin(); it != tag_map_it->second.end(); ++it) {
					std::shared_ptr<File> f = FileLoadManager::instance().LoadFile(it->file_name);
					if (f) {
						std::shared_ptr<ResourceBase> new_res = _CreateResource(it->type, f);
						if (new_res == nullptr) {
							debug::SystemLog::instance().LogError(std::string("���\�[�X�̍쐬�Ɏ��s���܂����B(") + "�^�O�O���[�v\"" + tag + "\"��\"" + it->file_name + "\")", "ResourceManager::_load_tag_group");
						}
						else {
							_using_resources.emplace(it->id, new_res);
							ids.push_back(it->id);
						}
					}
					else {
						debug::SystemLog::instance().LogError(std::string("���\�[�X�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(") + "�^�O�O���[�v\"" + tag + "\"��\"" + it->file_name + "\")", "ResourceManager::_load_tag_group");
					}
				}
				_using_tag_id_map.emplace(tag, std::move(ids));
			}
			return true;
		}

		bool ResourceManager::_LoadResourceList()
		{
			std::shared_ptr<file_system::File> file = file_system::FileLoadManager::instance().LoadFile(_resource_list_file_name);
			if (file == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X���X�g(") + _resource_list_file_name + ")�̓ǂݍ��݂Ɏ��s���܂����B","ResourceManager::_LoadResourceList");
				return false; 
			}
			if (file->GetStatus() != file_system::File::FileStatus::Available) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X���X�g�t�@�C����ǂݍ��߂܂���ł����B(" + _resource_list_file_name + ")") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "��)", "ResourceManager::_LoadResourceList");
				return false;
			}
			auto csv = MakeResource<resources::CSVResource>();
			if (csv->Create(file) == false) {
				return false; 
				debug::SystemLog::instance().LogError(std::string("���\�[�X���X�g�t�@�C����CSV�`���Ƃ��ēǂݍ��߂܂���ł����B(" + _resource_list_file_name + ")") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "��)", "ResourceManager::_LoadResourceList");
			}
			for (const auto& l : *csv) {
				ResourceData rd;
				std::vector<std::string> tags;
				if (l.size() < 4) { 
					std::string line;
					for (auto it = l.begin(); it != l.end();++it) {
						line += *it;
						if (it != l.end() - 1) { line += ","; }
					}
					debug::SystemLog::instance().LogWarning(std::string("���\�[�X��`�̍��ڐ�������܂���B���̍s�̓X�L�b�v���܂��B(") + line + ")", "ResourceManager::_LoadResourceList");
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
			debug::SystemLog::instance().LogMessage(std::string("���\�[�X���X�g��ǂݍ��݂܂����B(") + boost::lexical_cast<std::string>(_tag_resouce_map.size()) + "�̃^�O�O���[�v�A���v" + boost::lexical_cast<std::string>(total_resource_num) + "�̃��\�[�X��`)", "ResourceManager::_LoadResourceList");
			return true;
		}

		bool ResourceManager::Initialize()
		{
			if (_LoadResourceList() == false) {
				debug::SystemLog::instance().LogError("�������Ɏ��s���܂����B", "ResourceManagerInitialize");
				return false; 
			}
			debug::SystemLog::instance().LogMessage("���������܂����B", "ResourceManager::Initialize");
			return true;
		}

		bool ResourceManager::Finalize()
		{
			_UnloadAllLoadedResources();
			debug::SystemLog::instance().LogMessage("�I���������s���܂����B", "ResourceManager::Finalize");
			return true;
		}

		void ResourceManager::_UnloadAllLoadedResources()
		{
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
			debug::SystemLog::instance().LogMessage("���ׂẴ��\�[�X���A�����[�h���܂����B", "ResourceManager::_UnloadAllLoadedResources");
		}

		void ResourceManager::_AddUnusedtagGroups(const std::string& tag)
		{
			auto unused_map_it = _unused_tag_map.find(tag);
			if (unused_map_it == _unused_tag_map.end()) {
				_unused_tag_map.emplace(tag, std::vector<std::pair<std::string, std::shared_ptr<ResourceBase>>>());
				unused_map_it = _unused_tag_map.find(tag);
			}
			else { return; }
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

		std::shared_ptr<ResourceBase> ResourceManager::GetResource(const std::string& id)
		{
			auto it = _using_resources.find(id);
			if (it == _using_resources.end()) {
				debug::SystemLog::instance().LogWarning(std::string("�ǂݍ��܂�Ă��Ȃ����\�[�X���v������܂����B(ID:") + id + ")", "ResourceManager::GetResource");
				return nullptr;
			}
			else {
				return it->second;
			}
		}

		const std::unordered_set<std::string> ResourceManager::GetUsingTagGroups() const
		{
			std::unordered_set<std::string> out;
			for (const auto& t : _using_tag_id_map) {
				out.insert(t.first);
			}
			return std::move(out);
		}

		std::pair<std::vector<std::string>, std::vector<std::string>> ResourceManager::_check_newuse_and_nouse_tag_groups(const std::vector<std::string>& need_tags)
		{
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

	}
}