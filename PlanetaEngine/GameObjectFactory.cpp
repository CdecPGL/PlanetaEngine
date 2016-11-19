#include "Game.h"
#include "IResourceManager.h"
#include "GameObjectFactory.h"
#include "PrefixUtility.h"
#include "Reflection.h"
#include "GameObjectBase.h"
#include "RPtree.h"

#include "boost/algorithm/string.hpp"

namespace planeta {
	namespace private_ {
		std::shared_ptr<GameObjectBase> GameObjectFactory::GetNewGameObject(const std::string& game_object_def_file_id, const WeakPointer<private_::ISceneInternal>& scene_data) {
			//テンプレート
			std::shared_ptr<GameObjectBase> go_temp;
			//必要ならテンプレートを作成登録し、使用するテンプレートをセットする
			auto gtit = game_object_templates_.find(game_object_def_file_id);
			if (gtit == game_object_templates_.end()) {
				go_temp = CreateGameObjectFromFile_(game_object_def_file_id, scene_data);
				if (go_temp == nullptr) { return nullptr; }
				game_object_templates_.emplace(game_object_def_file_id, go_temp);
			} else {
				go_temp = gtit->second;
			}
			assert(go_temp != nullptr);
			//テンプレートをクローンし、返す
			auto ngo = CloneGameObjectFromTemplate_(go_temp, scene_data);
			if (ngo == nullptr) {
				PE_LOG_ERROR("GameObjectテンプレート(GameObject定義ファイルID:\"", game_object_def_file_id, "\")からのクローン作製に失敗しました。");
			}
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectFactory::GetNewGameObject(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data) {
			return CreateGameObjectFromComponentTypeList_(game_object_component_type_id_list, scene_data);
		}

		std::shared_ptr<GameObjectBase> GameObjectFactory::CreateGameObjectFromComponentTypeList_(const std::vector<std::string>& game_object_component_type_id_list, const WeakPointer<private_::ISceneInternal>& scene_data) {
			auto go_info = util::ConvertAndConnectToString("ゲームオブジェクトコンポーネントリスト:", boost::algorithm::join(game_object_component_type_id_list, ","));
			//生成
			auto ngo = std::make_shared<GameObjectBase>();
			if (ngo == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト(", go_info, ")の作成に失敗しました。");
				return nullptr;
			}
			//シーンデータセット
			ngo->SetSceneInternalInterface(scene_data);
			//コンポーネントの追加と設定
			if (!ngo->AddAndSetUpComponents(game_object_component_type_id_list)) {
				PE_LOG_ERROR("ゲームオブジェクト(", go_info, ")のコンポーネントの追加または設定に失敗しました。");
				return nullptr;
			}
			//成功
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectFactory::CreateGameObjectFromFile_(const std::string& game_object_def_file_id, const WeakPointer<private_::ISceneInternal>& scene_data) {
			//生成
			auto ngo = std::make_shared<GameObjectBase>();
			if (ngo == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト(GameObject定義ファイルID:\"", game_object_def_file_id, "\")の作成に失敗しました。");
				return nullptr;
			}
			//シーンデータセット
			ngo->SetSceneInternalInterface(scene_data);
			//ファイル読み込み
			auto ptree_res = Game::instance().resource_manager()->GetResourceByID<RPtree>(game_object_def_file_id);
			if (ptree_res == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト定義リソース\"", game_object_def_file_id, "\"の読み込みに失敗しました。");
				return nullptr;
			}
			//コンポーネントの追加と設定
			if (!ngo->AddAndSetUpComponents(*ptree_res->GetPtree())) {
				PE_LOG_ERROR("ゲームオブジェクトへファイル定義\"", game_object_def_file_id, "\"を読み込むことができませんでした。");
				return nullptr;
			}
			//成功
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectFactory::CloneGameObjectFromTemplate_(const std::shared_ptr<GameObjectBase>& go_temp, const WeakPointer<private_::ISceneInternal>& scene_data) {
			assert(go_temp != nullptr);
			//生成
			auto ngo = Reflection::CreateObjectByStdTypeInfo<GameObjectBase>(typeid(*go_temp));
			if (ngo == nullptr) {
				PE_LOG_ERROR("クローンの作成に失敗しました。");
				return nullptr;
			}
			//クローン時処理
			if (!ngo->ProcessClonation(go_temp)) {
				PE_LOG_ERROR("ゲームオブジェクトのクローン時処理に失敗しました。");
			}
			//成功
			return ngo;
		}

	}
}