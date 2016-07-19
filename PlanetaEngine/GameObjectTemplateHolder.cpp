#include "GameObjectTemplateHolder.h"
#include "PrefixUtility.h"
#include "Reflection.h"
#include "GameObjectBase.h"
#include "ResourceManager.h"
#include "RJson.h"

namespace planeta {
	namespace core {
		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::GetNewGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc) {
			//テンプレート
			std::shared_ptr<GameObjectBase> go_temp;
			//必要ならテンプレートを作成登録し、使用するテンプレートをセットする
			auto gtit = game_object_templates_.find(game_object_type_id);
			if (gtit == game_object_templates_.end()) {
				go_temp = CreateGameObject_(game_object_type_id, file_id, post_instantiate_proc);
				game_object_templates_.emplace(game_object_type_id, std::unordered_map<std::string, std::shared_ptr<GameObjectBase>>{ { file_id, go_temp } });
			} else {
				decltype(auto) fileid_temp_map = gtit->second;
				auto fit = fileid_temp_map.find(file_id);
				if (fit == fileid_temp_map.end()) {
					go_temp = CreateGameObject_(game_object_type_id, file_id, post_instantiate_proc);
					fileid_temp_map.emplace(file_id, go_temp);
				} else {
					go_temp = fit->second;
				}
			}
			//テンプレートをクローンし、返す
			auto ngo = go_temp->Clone();
			return ngo;
		}

		std::shared_ptr<GameObjectBase> GameObjectTemplateHolder::CreateGameObject_(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc) {
			auto obj_type_id = core::AddPrefix(game_object_type_id, core::ObjectCategory::GameObject);
			//生成
			auto ngo = Reflection::CreateObjectByID<GameObjectBase>(obj_type_id);
			if (ngo == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクトテンプレート(GameObjectTypeID:\"", game_object_type_id, "\"の作成に失敗しました。");
				return nullptr;
			}
			//インスタンス化時処理
			if (!ngo->ProcessInstantiation()) {
				PE_LOG_ERROR("ゲームオブジェクトテンプレート(GameObjectTypeID:\"", game_object_type_id, "\"のインスタンス化時処理に失敗しました。");
			}
			//インスタンス化後処理
			post_instantiate_proc(*ngo);
			//ファイルが指定されていないなら、読み込みは行わない。
			if (file_id.empty()) {
				return ngo;
			}
			//ファイル読み込み
			auto json_res = ResourceManager::instance().GetResource<RJson>(file_id);
			if (json_res == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクト定義リソース\"", file_id, "\"の読み込みに失敗しました。");
				return nullptr;
			}
			try {
				if(!ngo->ProcessLoading(*json_res->GetRoot().GetWithException<JSONObject>())) {
					PE_LOG_ERROR("ゲームオブジェクトへファイル定義\"",file_id,"\"を読み込むことができませんでした。");
					return nullptr;
				}
			} catch (JSONTypeError& e) {
				PE_LOG_ERROR("ゲームオブジェクト定義リソースのJSONファイルは、ルートValueがObjectである必要があります。");
				return nullptr;
			}
			//成功
			return ngo;
		}

	}
}