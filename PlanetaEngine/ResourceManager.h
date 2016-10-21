#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <thread>
#include <memory>
#include "Object.h"
#include "SingletonTemplate.h"
#include "MakeResource.h"
#include "SystemLog.h"

namespace planeta{
	class FileAccessor;
	class File;
	namespace private_{
		class ResourceBase;
		/*Resource管理クラス。非同期読み込みは未実装だが、実装時に変更が必要ないように非同期読み込みと同じように扱う。
		PrepairResourcesでリソースの準備を開始し、IsReadyがtrueになるまで待つ。そしてUnloadUnusedResourscesを呼ぶ。
		現状ではIsReadyは常にtrue、GetPrepairProgressは常に1.0を返す。
		*/
		class ResourceManager : public util::SingletonTemplate<ResourceManager>{
			friend util::SingletonTemplate<ResourceManager>;
		public:
			bool Initialize()override;
			void Finalize()override;
			/*アンロード対象外のタグを設定*/
			bool SetNotUnloadTags(const std::set<std::string>& tags);
			/*タグで指定されたリソースをまとめて読み込む*/
			bool PrepareResources(const std::vector<std::string>& need_tag_groups);
			/*未使用のタリソースをアンロードする*/
			bool UnloadUnusedResouces();
			/*Resourceの準備が完了したか*/
			bool IsReady()const;
			/*準備進行度(読み込みしていない時は1.0とする)*/
			double GetPrepairProgress()const;
			/*リソースの属性を追加*/
			template<class C>
			void AddResourceType(const std::string& type_name) {
				AddResourceCreatorMap_(type_name, [](const std::shared_ptr<const File>& file)->std::shared_ptr<ResourceBase> {
					std::shared_ptr<ResourceBase> new_res = MakeResource<C>();
					return new_res->Create(*file) ? new_res : nullptr;
				});
			}
			/*リソースをIDで取得*/
			std::shared_ptr<ResourceBase> GetResourceByID(const std::string& id);
			template<class RT>
			std::shared_ptr<RT> GetResourceByID(const std::string& id) {
				static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
				auto rsc = GetResourceByID(id);
				if (rsc) {
					auto out = std::dynamic_pointer_cast<RT>(rsc);
					if (out) {
						return out;
					} else {
						PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(RT).name(), "\")");
						return nullptr;
					}
				} else {
					return nullptr;
				}
			}
			/*リソースをパスで取得*/
			std::shared_ptr<ResourceBase> GetResourceByPath(const std::string& path, const std::string& root_path = "");
			template<class RT>
			std::shared_ptr<RT> GetResourceByPath(const std::string& path, const std::string& root_path = "") {
				static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
				auto rsc = GetResourceByPath(path, root_path);
				if (rsc) {
					auto out = std::dynamic_pointer_cast<RT>(rsc);
					if (out) {
						return out;
					} else {
						PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(RT).name(), "\")");
						return nullptr;
					}
				} else {
					return nullptr;
				}
			}
			/*リソースをIDかパスで取得(重複時はIDが優先される)*/
			std::shared_ptr<ResourceBase> GetResourceByIDorPath(const std::string& id_or_path, const std::string& root_path = "");
			template<class RT>
			std::shared_ptr<RT> GetResourceByIDorPath(const std::string& id_or_path, const std::string& root_path = "") {
				static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
				auto rsc = GetResourceByIDorPath(id_or_path, root_path);
				if (rsc) {
					auto out = std::dynamic_pointer_cast<RT>(rsc);
					if (out) {
						return out;
					} else {
						PE_LOG_ERROR("リソースの型を変換できませんでした。(\"ターゲット型:", typeid(RT).name(), "\")");
						return nullptr;
					}
				} else {
					return nullptr;
				}
			}
			/*ファイルアクセサをセット。初期化前に呼び出す*/
			void SetFileAccessor_(const std::shared_ptr<FileAccessor>& f_scsr);
			/*リソースリストファイル名を設定。初期化前に呼び出す必要がある*/
			void SetResourceListFileName_(const std::string& file_name);
		private:
			ResourceManager();
			ResourceManager(const ResourceManager&) = delete;
			~ResourceManager();
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			/*リソースクリエータ関数型*/
			using _ResourceCreatorType = std::function<std::shared_ptr<ResourceBase>(const std::shared_ptr<const File>&)>;
			void AddResourceCreatorMap_(const std::string& type_name, const _ResourceCreatorType& creator);
		};
	}
}