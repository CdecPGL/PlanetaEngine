#include"ResourceManager.h"
#include <thread>

namespace PlanetaEngine{
	namespace Resource{

		bool ResourceManager::PrepareResources(const std::vector<std::string>& need_tags)
		{
			auto ret = std::move(_check_newuse_and_nouse_tag_groups(need_tags));
			_nouse_tag_groups = std::move(ret.second); //アンロードするべきタグは、未使用タググループリストに登録
			auto load_tags = std::move(ret.first);
			//新規読み込み
			for (const auto& tag : load_tags){
				_load_tag_group(tag);
			}
			return true;
		}

		bool ResourceManager::UnloadUnusedResouces()
		{
			for (auto& tag : _nouse_tag_groups){
				_unload_tag_group(tag);
			}
			_nouse_tag_groups.clear();
			return true;
		}

		bool ResourceManager::PrepareResourcesAsynchronously(const std::vector<std::string>& need_tags)
		{
			if (_res_load_thread->joinable()){ return false; } //すでにスレッドが使われている
			else{
				_res_load_thread = std::make_unique<std::thread>([this, &need_tags]()->bool{return PrepareResources(need_tags); });
				return true;
			}
		}

		bool ResourceManager::IsLoading()const{ return _res_load_thread->joinable(); }

		bool ResourceManager::UnloadUnusedResoucesAsynchronously()
		{
			if (_res_load_thread->joinable()){ return false; } //すでにスレッドが使われている
			else{
				_res_load_thread = std::make_unique<std::thread>([this]()->bool{return UnloadUnusedResouces(); });
				return true;
			}
		}

	}
}