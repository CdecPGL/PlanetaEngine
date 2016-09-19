#include "GameObjectTemplateHolder.h"
#include "PrefixUtility.h"
#include "Reflection.h"
#include "GameObjectBase.h"
#include "ResourceManager.h"
#include "RPtree.h"

#include "SceneData.h"

namespace planeta {
	namespace private_ {
		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::GetNewGameObject(const std::string& game_object_type_id, const std::string& file_id, const WeakPointer<private_::SceneData>& scene_data) {
			//テンプレート
			std::shared_ptr<GameObjectBase> go_temp;
			//必要ならテンプレートを作成登録し、使用するテンプレートをセットする
			auto gtit = game_object_templates_.find(game_object_type_id);
			if (gtit == game_object_templates_.end()) {
				go_temp = CreateGameObject_(game_object_type_id, file_id, scene_data);
				if (go_temp == nullptr) { return nullptr; }
				game_object_templates_.emplace(game_object_type_id, std::unordered_map<std::string, std::shared_ptr<GameObjectBase>>{ { file_id, go_temp } });
			} else {
				decltype(auto) fileid_temp_map = gtit->second;
				auto fit = fileid_temp_map.find(file_id);
				if (fit == fileid_temp_map.end()) {
					go_temp = CreateGameObject_(game_object_type_id, file_id, scene_data);
					if (go_temp == nullptr) { return nullptr; }
					fileid_temp_map.emplace(file_id, go_temp);
				} else {
					go_temp = fit->second;
				}
			}
			assert(go_temp != nullptr);
			//テンプレートをクローンし、返す
			auto ngo = CloneGameObjectFromTemplate_(go_temp, scene_data);
			if (ngo == nullptr) {
				PE_LOG_ERROR("GameObjectテンプレート(GameObjectTypeID:\"", game_object_type_id, "\", file_id:\"", file_id ,"\")からのクローン作製に失敗しました。");
			}
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::CreateGameObject_(const std::string& game_object_type_id, const std::string& file_id, const WeakPointer<private_::SceneData>& scene_data) {
			auto obj_type_id = private_::AddPrefix(game_object_type_id, private_::ObjectCategory::GameObject);
			//生成
			auto ngo = Reflection::CreateObjectByObjectTypeID<GameObjectBase>(obj_type_id);
			if (ngo == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト(GameObjectTypeID:\"", game_object_type_id, "\")の作成に失敗しました。");
				return nullptr;
			}
			//シーンデータセット
			ngo->SetSceneData(scene_data);
			//インスタンス化時処理
			if (!ngo->ProcessInstantiation()) {
				PE_LOG_ERROR("ゲームオブジェクト(GameObjectTypeID:\"", game_object_type_id, "\")のインスタンス化時処理に失敗しました。");
			}
			//コンポーネントにシーンとGOデータをセット
			ngo->SetSceneAndGODataToCOmponents();
			//ファイルが指定されていないなら、読み込みは行わない。
			if (file_id.empty()) {
				return ngo;
			}
			//ファイル読み込み
			auto ptree_res = ResourceManager::instance().GetResource<RPtree>(file_id);
			if (ptree_res == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト定義リソース\"", file_id, "\"の読み込みに失敗しました。");
				return nullptr;
			}
			if (!ngo->ProcessLoading(*ptree_res->GetPtree())) {
				PE_LOG_ERROR("ゲームオブジェクトへファイル定義\"", file_id, "\"を読み込むことができませんでした。");
				return nullptr;
			}
			//成功
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::CloneGameObjectFromTemplate_(const std::shared_ptr<GameObjectBase>& go_temp, const WeakPointer<private_::SceneData>& scene_data) {
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