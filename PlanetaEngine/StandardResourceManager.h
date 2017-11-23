#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <thread>
#include <memory>
#include "Object.h"
#include "LogUtility.h"
#include "ResourceManager.h"

namespace planeta{
	class FileAccessor;
	class File;
	namespace private_{
		/*標準のResource管理クラス。非同期読み込みは未実装だが、実装時に変更が必要ないように非同期読み込みと同じように扱う。
		PrepairResourcesでリソースの準備を開始し、IsReadyがtrueになるまで待つ。そしてUnloadUnusedResourscesを呼ぶ。
		現状ではIsReadyは常にtrue、GetPrepairProgressは常に1.0を返す。
		*/
		class StandardResourceManager final: public ResourceManager{
		public:
			StandardResourceManager();
			StandardResourceManager(const StandardResourceManager&) = delete;
			~StandardResourceManager();
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
			std::shared_ptr<ResourceBase> GetResourceByID(const std::string& id)override;
			/*ファイルアクセサをセット。初期化前に呼び出す*/
			void SetFileManipulator_(const std::shared_ptr<FileManipulatorBase>& f_scsr)override;
			/*リソースリストファイル名を設定。初期化前に呼び出す必要がある*/
			void SetResourceListFileName_(const std::string& file_name)override;
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			void AddResourceCreator(const std::string& type_name, const ResourceCreatorType& creator)override;
		};
	}
}