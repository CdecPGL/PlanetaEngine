#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <list>
#include <typeinfo>
#include <typeindex>

#include "boost/type_index.hpp"

#include "LogUtility.h"
#include "ResourceManager.h"

namespace planeta {
	class FileAccessor;
	class File;
	namespace private_ {
		class ResourceManagerInternalAccessor;
		/*標準のResource管理クラス。非同期読み込みは未実装だが、実装時に変更が必要ないように非同期読み込みと同じように扱う。
		PrepairResourcesでリソースの準備を開始し、IsReadyがtrueになるまで待つ。そしてUnloadUnusedResourscesを呼ぶ。
		現状ではIsReadyは常にtrue、GetPrepairProgressは常に1.0を返す。
		*/
		class StandardResourceManager final : public ResourceManager {
		public:
			StandardResourceManager();
			~StandardResourceManager()override;
			bool Initialize()override;
			void Finalize()override;
			/*アンロード対象外のタグを設定*/
			bool SetNotUnloadTags(const std::set<std::string>& tags)override;
			/*タグで指定されたリソースをまとめて読み込む*/
			bool PrepareResources(const std::vector<std::string>& need_tag_groups)override;
			/*未使用のタリソースをアンロードする*/
			bool UnloadUnusedResouces()override;
			/*Resourceの準備が完了したか*/
			bool IsReady()const override;
			/*準備進行度(読み込みしていない時は1.0とする)*/
			double GetPrepairProgress()const override;
			/*リソースをIDで取得*/
			std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info& type, const std::string& id)override;
			/*リソースをFullIDで取得*/
			std::shared_ptr<ResourceBase> GetResourceByFullID(const std::string& full_id)override;
			/*ファイルアクセサをセット。初期化前に呼び出す*/
			void SetFileManipulator_(const std::shared_ptr<FileManipulator>& f_scsr)override;
			/*リソースリストファイル名を設定。初期化前に呼び出す必要がある*/
			void SetResourceListFileName_(const std::string& file_name)override;
		private:
			void OnResourceTypeAdded(const std::type_info& type, const std::string& type_name, const std::string& type_prefix, const ResourceCreatorType& creator)override;

			std::shared_ptr<FileManipulator> file_accessor_;
			/*リソースリストのファイル名*/
			std::string resource_list_file_name_;

			struct ResourceData_ {
				/*種類*/
				std::type_index type_index;
				/*ID*/
				std::string id = "";
				/*フルID*/
				std::string full_id = "";
				/*ファイルパス*/
				std::string file_path = "";
				/*メタデータファイルパス*/
				std::string metadata_file_path = "";
				/*リソース本体*/
				std::shared_ptr<ResourceBase> resouce;
				/*リソースがロードされているか*/
				bool is_loaded = false;
				/*タグ*/
				std::vector<std::string> tags;
				/*アンロード対象外か*/
				bool not_unload = false;
			};
			using ResourceDataListType_ = std::list<ResourceData_>;
			ResourceDataListType_ resource_data_list_;
			std::unordered_map<std::string, std::vector<ResourceDataListType_::iterator>> tag_map_;
			std::unordered_map<std::string, ResourceDataListType_::iterator> full_id_map_;
			std::unordered_map<std::string, ResourceDataListType_::iterator> path_map_;
			std::set<std::string> not_unload_tags_;

			struct ResourceTypeData_ {
				boost::typeindex::type_index type;
				std::string type_name;
				std::string type_prefix;
				ResourceCreatorType creator;
			};
			/*ResourceのタイプによるResourceタイプデータ*/
			std::unordered_map<std::type_index, ResourceTypeData_> resource_type_data_map_;
			/*リソースタイプ接頭辞によるリソースタイプマップ*/
			std::unordered_map<std::string, std::type_index> resource_type_prefix_to_type_map_;
			/*リソースの作成*/
			std::shared_ptr<ResourceBase> CreateResource_(const std::type_index& type);
			/*リソースのロード*/
			std::shared_ptr<ResourceBase> LoadResource_(ResourceData_& res_dat);
			/*型とIDからFullIDを取得*/
			std::string GetFullIDFromTypeAndID_(const boost::typeindex::type_index& type_index, const std::string& id);
			/*リソースのアンロード*/
			void UnloadResource_(ResourceData_& res_dat);
			/*リソースデータの登録*/
			bool RegisterResourceData_(ResourceData_&& res_dat);
			/*アンロードタグの指定*/
			bool SetNotUnloadTags_(const std::set<std::string>& tags);
			/*リソースリストの読み込み*/
			bool LoadResourceList_();
			/*読み込まれているすべてのリソースをアンロードする*/
			void UnloadAllLoadedResources_();
			/*内部マネージャアクセサを取得*/
			ResourceManagerInternalAccessor CreateInternalManagerAccessor_();
			/*リソースをIDで取得する。ロードされていないリソース指定時に警告を出すか指定可能*/
			std::shared_ptr<ResourceBase> GetResourceByFullID_(const std::string& full_id, bool is_valid_not_preload_warning);
			/*リソースをパスで取得する。ロードされていないリソース指定時に警告を出すか指定可能*/
			std::shared_ptr<ResourceBase> GetResourceByPath_(const std::string& path, const std::string& root_path, bool is_valid_not_preload_warning);
		};
	}
}